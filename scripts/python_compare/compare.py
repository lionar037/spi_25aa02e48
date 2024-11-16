def compare_files(file1, file2):
    try:
        with open(file1, "rb") as f1, open(file2, "rb") as f2:
            offset = 0
            differences = 0

            while True:
                byte1 = f1.read(1)
                byte2 = f2.read(1)

                # Terminate when both files reach EOF
                if not byte1 and not byte2:
                    break

                # Check if bytes differ
                if byte1 != byte2:
                    differences += 1
                    print(f"Difference at offset {offset}: {byte1} != {byte2}")

                offset += 1

            if differences == 0:
                print("Files are identical.")
            else:
                print(f"Total differences found: {differences}")

    except FileNotFoundError as e:
        print(f"Error: {e}")
    except Exception as e:
        print(f"An unexpected error occurred: {e}")


# Replace with your file paths
file1 = "25vf010a.bin"
file2 = "st25vf010a_flashrom.bin"

compare_files(file1, file2)
