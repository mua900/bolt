from pathlib import Path
import argparse
import os.path

def dump_header(contents : bytes, variable_name : str, file_name : str, directory : Path):
    with open(directory / file_name, "w") as f:
        f.write("unsigned char " + variable_name + "[] = {\n")
        fullLines : int = len(contents) // 8
        for line in range(fullLines):
            f.write("\t")
            for i in range(8):
                f.write(f"0x{contents[line * 8 + i]:02x}, ")
            f.write("\n");
        f.write("\t")
        for i in range(len(contents) % 8):
            f.write(f"0x{contents[fullLines * 8 + i]:02x}, ")
        f.write("\n")
        f.write("};\n")
        f.write("unsigned int " + variable_name + "_len" + f" = {len(contents)};\n")

def generate_header(file : str, var : str):
    # output
    directory : Path = Path(os.path.normpath(__file__)).parent.parent / "src/bundle"
    var_name : str = None
    if var:
        var_name = var
    else:
        path = Path(file)
        var_name = path.stem + "_" + path.suffix[1:]

    print(var_name)

    data : bytes
    with open(file, "rb") as f:
        data = f.read()

    dump_header(data, var_name, var_name + ".h", directory)

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('-var', type=str)
    parser.add_argument('file', type=str)

    args = parser.parse_args()

    generate_header(args.file, args.var)

if __name__ == "__main__":
    main()
