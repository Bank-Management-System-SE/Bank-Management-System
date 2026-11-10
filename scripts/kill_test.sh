#!/usr/bin/env bash
# Interrupted-transfer test for BMS-SR-005 / TC-SEC-05.
# Run from the repository root on Linux or WSL.

set -u

[ -f tests/kill_harness.cpp ] || {
    echo "Run this from the repo root."
    exit 1
}

BK=$(mktemp -d)
DATA_EXISTED=0
RESTORED=0

if [ -d data ]; then
    DATA_EXISTED=1
    cp -a data "$BK/data_backup"
fi

cleanup() {
    if [ "$RESTORED" -eq 0 ]; then
        rm -rf data
        if [ "$DATA_EXISTED" -eq 1 ]; then
            mv "$BK/data_backup" data
        else
            mkdir -p data
        fi
    fi
    rm -rf "$BK"
}

trap cleanup EXIT INT TERM

rm -f data/accounts.dat data/txns.dat data/audit.log data/accounts.tmp
mkdir -p data

LIB=$(find src -maxdepth 1 -name '*.cpp' ! -name 'main.cpp' -print)

g++ -std=c++17 -O2 -Iinclude \
    tests/kill_harness.cpp $LIB -o "$BK/kill_harness" || {
    echo "Build failed"
    exit 1
}

"$BK/kill_harness" init > /dev/null || {
    echo "Could not create the test accounts"
    exit 1
}

RUNS=40
BAD=0
LAG=0

for run in $(seq 1 "$RUNS"); do
    ms=$(( RANDOM % 300 + 20 ))

    timeout -s KILL "$(printf '0.%03d' "$ms")" \
        "$BK/kill_harness" run 1000000 >/dev/null 2>&1 || true

    TOTAL=$(awk -F'|' '{s+=$5} END {printf "%.2f", s}' data/accounts.dat)
    MALFORMED=$(awk -F'|' 'NF!=8' data/accounts.dat | wc -l)

    if [ "$TOTAL" != "1500.00" ] || [ "$MALFORMED" != "0" ]; then
        BAD=$((BAD+1))
        echo "run $run: INCONSISTENT total=$TOTAL malformed_lines=$MALFORMED"
    fi

    for acc in 1001 1002; do
        STORED=$(awk -F'|' -v a="$acc" '$1==a{printf "%.2f",$5}' data/accounts.dat)
        LAST=$(grep "|$acc|" data/txns.dat | tail -1 | awk -F'|' '{printf "%.2f",$6}')
        if [ "$STORED" != "$LAST" ]; then
            LAG=$((LAG+1))
        fi
    done
done

ROWS=$(grep -c "TRANSFER_" data/txns.dat || true)

echo "Runs killed mid-transfer              : $RUNS"
echo "Runs with inconsistent balances       : $BAD"
echo "Transfer ledger rows written          : $ROWS"
echo "Accounts whose last ledger row lagged : $LAG"

if [ "$ROWS" -le 0 ]; then
    echo "RESULT: INVALID - no transfers ran"
    BAD=1
elif [ "$BAD" -eq 0 ]; then
    echo "RESULT: PASS - no inconsistent balances detected"
else
    echo "RESULT: FAIL"
fi

if [ "$BAD" -ne 0 ]; then
    exit 1
fi
