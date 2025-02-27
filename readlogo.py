
import sys

def convert_bmp_to_array(filename):
    with open(filename, "rb") as f:
        data = f.read()

    hex_array = ", ".join(f"0x{byte:02X}" for byte in data)
    
    c_code = f"""#ifndef LOGO_DATA_H
#define LOGO_DATA_H

const unsigned char logo_data[] = {{
    {hex_array}
}};

const unsigned int logo_size = {len(data)};

#endif // LOGO_DATA_H
"""
    
    with open("logo_data.h", "w") as f:
        f.write(c_code)

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python bmp_to_c_array.py logo.bmp")
    else:
        convert_bmp_to_array(sys.argv[1])
