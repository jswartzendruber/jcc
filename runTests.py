#!/bin/python3

import subprocess
import json
import os

passed = 0
failed = 0
for file in os.listdir("tests"):
    filename = "tests/" + os.fsdecode(file)

    f = open(filename)
    data = json.load(f)

    arguments = ["./jcc"]
    for inp in data['input']:
        arguments.append(inp)

    output_len = len(data['output'])

    expected_output = []
    for line in data['output']:
        expected_output.append(line)

    f.close()

    if output_len == 1:
        [expected_output] = expected_output
    elif output_len == 0:
        expected_output = ""

    actual_output = str(subprocess.run(arguments, capture_output=True, text=True).stdout)

    if actual_output == expected_output:
        print(filename + " . . . \033[92;1mPASS\033[0m")
        passed += 1
    else:
        print(filename + " . . . \033[91;1mFAIL\033[0m")
        failed += 1

print()
passed_text = "tests passed"
if passed == 1:
    passed_text = "test passed"
print("\033[92;1m" + str(passed) + "\033[0m " + passed_text + ", \033[91;1m" + str(failed) + "\033[0m failed.")
