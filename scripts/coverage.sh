#!/usr/bin/env bash
set -u

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
OUT="${ROOT}/coverage"
CXX="${CXX:-}"
GCOV="${GCOV:-}"
CXXFLAGS="-std=c++17 -Wall -Wextra -I${ROOT}/include -O0 -g --coverage"
if [[ -z "${CXX}" ]]; then
    if command -v g++ >/dev/null 2>&1; then CXX="$(command -v g++)"; else CXX="/c/MinGW/bin/g++.exe"; fi
fi
if [[ -z "${GCOV}" ]]; then
    if command -v gcov >/dev/null 2>&1; then GCOV="$(command -v gcov)"; else GCOV="/c/MinGW/bin/gcov.exe"; fi
fi
mapfile -t LIBSRC < <(find "${ROOT}/src" -maxdepth 1 -name '*.cpp' ! -name 'main.cpp' -print)
mapfile -t TESTS < <(find "${ROOT}/tests" -maxdepth 1 -name 'test_*.cpp' -print | sort)

rm -rf "${OUT}"
mkdir -p "${OUT}"
rm -f "${ROOT}"/*.gcda "${ROOT}"/*.gcno

passed=0
failed=0
for test in "${TESTS[@]}"; do
    name=$(basename "${test}" .cpp)
    binary="${OUT}/${name}"
    echo "=== ${name} ==="
    if ! "${CXX}" ${CXXFLAGS} -o "${binary}" "${test}" "${LIBSRC[@]}"; then
        echo "BUILD FAILED: ${name}"
        failed=$((failed + 1))
        continue
    fi
    if "${binary}"; then
        passed=$((passed + 1))
    else
        echo "TEST FAILED: ${name}"
        failed=$((failed + 1))
    fi
done

{
    echo "# Branch coverage report"
    echo
    echo "Generated: $(date -u '+%Y-%m-%d %H:%M:%SZ')"
    echo "Tests passed: ${passed}"
    echo "Tests failed: ${failed}"
    echo
    echo "## Per-file coverage"
    if compgen -G "${OUT}/*.gcda" > /dev/null; then
        (cd "${OUT}" && "${GCOV}" -b -c -l -p *.gcno 2>/dev/null) \
          | awk '/^File / || /Lines executed:/ || /Branches executed:/ || /Taken at least once:/ {print}'
        echo
        echo "## Branch-by-branch details"
        echo "Format: source file : line : branch outcome/taken count : coverage status"
        for gcov_file in "${OUT}"/*.gcov; do
            [ -f "${gcov_file}" ] || continue
            awk '
                /^[[:space:]-]+:[[:space:]]*0:Source:/ {
                    sub(/^.*:Source:/, ""); source=$0
                }
                /^[[:space:]]*(#####[^:]*|[0-9]+):[[:space:]]*[0-9]+:/ {
                    split($0, fields, ":"); current_line=fields[2]
                }
                /^[[:space:]]*branch[[:space:]]+[0-9]+[[:space:]]+taken/ {
                    outcome=$0
                    if (source !~ /\/(src|include)\//) next
                    status=(outcome ~ /taken[[:space:]]+0([[:space:]]|%)/ || outcome ~ /taken[[:space:]]+-/) ? "Not covered" : "Covered"
                    printf "%s:%s: %s : %s\n", source, current_line, outcome, status
                }
            ' "${gcov_file}"
        done
    else
        echo "No .gcda files were produced; compilation or execution was unavailable."
    fi
} > "${OUT}/branch-coverage.txt"

echo
cat "${OUT}/branch-coverage.txt"
exit $(( failed > 0 ? 1 : 0 ))
