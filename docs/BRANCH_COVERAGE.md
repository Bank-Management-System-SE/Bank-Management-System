# Branch coverage

Run the report from the repository root with:

```sh
bash scripts/coverage.sh
```

The script compiles each `tests/test_*.cpp` target with GCC coverage
instrumentation, runs every target, and writes the generated report to
`coverage/branch-coverage.txt`. Coverage artifacts are intentionally kept out
of version control; only the reproducible script and this usage document are
committed.

The report includes the test pass/fail count and the `gcov -b` branch metrics
for each instrumented source file. A non-zero exit status means at least one
test target failed, while still preserving the report for diagnosis.
