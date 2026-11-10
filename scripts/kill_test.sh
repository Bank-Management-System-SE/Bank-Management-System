#!/usr/bin/env bash
# Interrupted-transfer test for BMS-SR-005 / TC-SEC-05. Run from the repo root on Linux or WSL.
# It needs only g++ (no "make bms", no UI). It backs up and restores your data/ folder.
#
# What it does: builds tests/kill_harness.cpp against the real source files, creates two accounts
# (1000 + 500 = 1500), then repeatedly starts the harness (thousands of real transfer() calls)
# and KILLS the process after a random 20-320 ms. After every kill:
#   - the total of all balances must be exactly 1500.00 and accounts.dat must be readable  (must never fail)
#   - the last ledger row of each account is compared with the stored balance (informational:
#     a mismatch is the known limitation that a ledger row can be missing after a crash)
set -u
[ -f tests/kill_harness.cpp ] || { echo "Run this from the repo root (tests/kill_harness.cpp not found)."; exit 1; }
BK=$(mktemp -d)
[ -d data ] && cp -r data "$BK/data_backup"
rm -f data/accounts.dat data/txns.dat data/audit.log data/accounts.tmp
mkdir -p data

LIB=$(ls src/*.cpp | grep -v 'src/main.cpp')
g++ -std=c++17 -O2 -Iinclude tests/kill_harness.cpp $LIB -o "$BK/kill_harness" || { echo "Build failed"; exit 1; }
"$BK/kill_harness" init > /dev/null || { echo "Could not create the test accounts"; exit 1; }

RUNS=40; BAD=0; LAG=0
for run in $(seq 1 $RUNS); do
  ms=$(( RANDOM % 300 + 20 ))
  ( timeout -s KILL "$(printf '0.%03d' $ms)" "$BK/kill_harness" run 1000000 > /dev/null 2>&1 ) 2>/dev/null
  TOTAL=$(awk -F'|' '{s+=$5} END {printf "%.2f", s}' data/accounts.dat)
  MALFORMED=$(awk -F'|' 'NF!=8' data/accounts.dat | wc -l)
  if [ "$TOTAL" != "1500.00" ] || [ "$MALFORMED" != "0" ]; then
    BAD=$((BAD+1)); echo "run $run: INCONSISTENT  total=$TOTAL  malformed_lines=$MALFORMED"
  fi
  for acc in 1001 1002; do
    STORED=$(awk -F'|' -v a=$acc '$1==a{printf "%.2f",$5}' data/accounts.dat)
    LAST=$(grep "|$acc|" data/txns.dat | tail -1 | awk -F'|' '{printf "%.2f",$6}')
    [ "$STORED" != "$LAST" ] && LAG=$((LAG+1))
  done
done

ROWS=$(grep -c "TRANSFER_" data/txns.dat)
echo "-------------------------------------------------------------"
echo "Runs killed mid-transfer              : $RUNS"
echo "Runs with inconsistent balances       : $BAD   (must be 0)"
echo "Transfer ledger rows written          : $ROWS"
echo "Accounts whose last ledger row lagged : $LAG   (known limitation, see DEVIATIONS.md)"
if [ "$ROWS" -le 0 ]; then
  echo "RESULT: INVALID - no transfers ran"; BAD=1
elif [ "$BAD" = "0" ]; then
  echo "RESULT: PASS - no one-sided or corrupted balance after any kill"
else
  echo "RESULT: FAIL"
fi

rm -rf data
[ -d "$BK/data_backup" ] && mv "$BK/data_backup" data || mkdir -p data
rm -rf "$BK"
[ "$BAD" = "0" ]
