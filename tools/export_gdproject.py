import os
import subprocess
import sys

if len(sys.argv) != 3:
    print(f"Usage: {sys.argv[0]} <project> <target>")
    exit(1)

(project, target) = sys.argv[1:]
debug = "debug" in target
project = os.path.abspath(project)
output_dir = os.path.join(project, "export", target)
os.makedirs(output_dir, exist_ok=True)

subprocess.call(
    [
        "godot",
        "--headless",
        "--path",
        project,
        "--export-debug" if debug else "--export-release",
        target,
        os.path.join(project, "export", target, target),
    ]
)
