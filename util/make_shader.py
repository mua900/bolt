from pathlib import Path
import argparse
import os.path

import compile_shader
import generate_header

def guess_stage(filename : str) -> str:
    name_lower : str = filename.lower()
    if "vertex" in name_lower:
        return "vertex"
    elif "fragment" in name_lower:
        return "fragment"
    elif "compute" in name_lower:
        return "compute"

    return None

def main():
    parent : Path = Path(os.path.normpath(__file__)).parent.parent
    out_shader : Path = parent / "shader" / "binary"
    out_reflect : Path = parent / "shader" / "reflection"

    parser = argparse.ArgumentParser(description="Compile the provided shader and generate it's header files")
    parser.add_argument("shader", type=str, help="Path to shader source")
    parser.add_argument("-stage", type=str, default=None, choices=["vertex", "fragment", "compute"], help="Shader stage name")

    args = parser.parse_args()

    shader_path : Path = Path(args.shader)
    if not shader_path.exists():
        print("Shader file not found")
        return

    stage : str = args.stage
    if not stage:
        stage = guess_stage(shader_path.name)
        if not stage:
            print("Could not figure out the shader stage")
            return

    print(f"shader stage: {stage}")

    if not compile_shader.compile_shaders([str(shader_path)], stage, out_shader, out_reflect):
        print("Couldn't compile shader")
        return

    shader_name = shader_path.stem
    for ext in [".spv", ".dxil", ".msl"]:
        out_file : str = f"{shader_name}{ext}"
        generate_header.generate_header(str(out_shader / out_file), None)

if __name__ == "__main__":
    main()