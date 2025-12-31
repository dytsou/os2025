#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

uint32_t swap_endian(uint32_t val)
{
    return ((val >> 24) & 0x000000FF) |
           ((val >> 8) & 0x0000FF00) |
           ((val << 8) & 0x00FF0000) |
           ((val << 24) & 0xFF000000);
}

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        printf("Usage: %s <image file> <output file>\n", argv[0]);
        exit(1);
    }

    FILE *input_file = fopen(argv[1], "rb");
    if (input_file == NULL)
    {
        fprintf(stderr, "Error: Cannot open input file %s\n", argv[1]);
        exit(1);
    }

    FILE *output_file = fopen(argv[2], "a");
    if (output_file == NULL)
    {
        fprintf(stderr, "Error: Cannot open output file %s\n", argv[2]);
        fclose(input_file);
        exit(1);
    }

    // Read PNG magic bytes: 0x89 0x50 0x4E 0x47
    uint8_t magic[8];
    if (fread(magic, 1, 8, input_file) != 8)
    {
        // Not a valid PNG file
        fprintf(output_file, "# %s\n\n\n---\n\nFileType: Unknown\n\n\n", argv[1]);
        fclose(input_file);
        fclose(output_file);
        return 0;
    }

    // Check PNG magic: 0x89 0x50 0x4E 0x47 (PNG signature)
    if (magic[0] != 0x89 || magic[1] != 0x50 || magic[2] != 0x4E || magic[3] != 0x47)
    {
        // Not a PNG file
        fprintf(output_file, "# %s\n\n\n---\n\nFileType: Unknown\n\n\n", argv[1]);
        fclose(input_file);
        fclose(output_file);
        return 0;
    }

    // Now we're at position 8, need to find IHDR chunk
    // PNG structure: [8-byte signature][chunk length][chunk type][chunk data][CRC]
    uint32_t chunk_length;
    char chunk_type[5];
    chunk_type[4] = '\0';
    
    while (1)
    {
        // Read chunk length (4 bytes, big-endian)
        if (fread(&chunk_length, 4, 1, input_file) != 1)
        {
            break;
        }
        chunk_length = swap_endian(chunk_length);

        // Read chunk type (4 bytes)
        if (fread(chunk_type, 1, 4, input_file) != 4)
        {
            break;
        }

        // Check if this is IHDR chunk
        if (strcmp(chunk_type, "IHDR") == 0)
        {
            // Read IHDR data: width (4 bytes), height (4 bytes), bit_depth (1 byte),
            // color_type (1 byte), compression (1 byte), filter (1 byte), interlace (1 byte)
            uint32_t width, height;
            uint8_t bit_depth, color_type;
            
            if (fread(&width, 4, 1, input_file) != 1)
                break;
            if (fread(&height, 4, 1, input_file) != 1)
                break;
            if (fread(&bit_depth, 1, 1, input_file) != 1)
                break;
            if (fread(&color_type, 1, 1, input_file) != 1)
                break;

            // Swap endianness for width and height
            width = swap_endian(width);
            height = swap_endian(height);

            // Skip remaining IHDR data (compression, filter, interlace = 3 bytes)
            fseek(input_file, 3, SEEK_CUR);

            // Read CRC (4 bytes, big-endian)
            uint32_t crc;
            if (fread(&crc, 4, 1, input_file) != 1)
                break;
            crc = swap_endian(crc);

            // Determine color type string
            const char *color_type_str;
            switch (color_type)
            {
            case 0:
                color_type_str = "Grayscale";
                break;
            case 2:
                color_type_str = "RGB";
                break;
            case 3:
                color_type_str = "PLTE";
                break;
            case 4:
                color_type_str = "GrayscaleAlpha";
                break;
            case 6:
                color_type_str = "RGBA";
                break;
            default:
                color_type_str = "Unknown";
                break;
            }

            // Write report to output file
            fprintf(output_file, "# %s\n", argv[1]);
            fprintf(output_file, "...\n");
            fprintf(output_file, "FileType: PNG\n");
            fprintf(output_file, "Size: %ux%u\n", width, height);
            fprintf(output_file, "Bit Depth: %u\n", bit_depth);
            fprintf(output_file, "Color Type: %s\n", color_type_str);
            fprintf(output_file, "IHDR CRC: %02x%02x%02x%02x\n", 
                    (crc >> 24) & 0xFF,
                    (crc >> 16) & 0xFF,
                    (crc >> 8) & 0xFF,
                    crc & 0xFF);
            fprintf(output_file, "...\n");
            fprintf(output_file, "\n");
            fprintf(output_file, "![(%s})\n", argv[1]);
            fprintf(output_file, "\n");
            fprintf(output_file, "\n");
            
            break;
        }
        else
        {
            // Skip this chunk: jump over chunk data and CRC (chunk_length + 4 bytes)
            fseek(input_file, chunk_length + 4, SEEK_CUR);
        }
    }

    fclose(input_file);
    fclose(output_file);
    return 0;
}

