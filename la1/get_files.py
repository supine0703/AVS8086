import os
import sys

def list_files(base_path):
    relative_paths = []
    for root, _, files in os.walk(base_path):
        for file in files:
            relative_path = os.path.relpath(os.path.join(root, file), base_path)
            relative_paths.append(relative_path)
    return relative_paths

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python script.py <path>")
    else:
        base_path = sys.argv[1]
        if os.path.exists(base_path):
            files = list_files(base_path)
            for file in files:
                print(file)
        else:
            print(f"Path '{base_path}' does not exist.")
