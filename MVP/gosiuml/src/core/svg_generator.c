#include "phenomemory_platform.h"
#include <stdio.h>

void generate_svg_from_tokens(PhenoToken* tokens, int count, const char* output_file) {
    FILE* svg = fopen(output_file, "w");
    if (!svg) {
        perror("Failed to create SVG file");
        return;
    }
    
    // SVG header
    fprintf(svg, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    fprintf(svg, "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"1200\" height=\"800\" viewBox=\"0 0 1200 800\">\n");
    fprintf(svg, "  <title>PhenoMemory State Machine Visualization</title>\n");
    
    // Background
    fprintf(svg, "  <rect width=\"1200\" height=\"800\" fill=\"#f5f5f5\"/>\n");
    
    // Title
    fprintf(svg, "  <text x=\"600\" y=\"30\" text-anchor=\"middle\" ");
    fprintf(svg, "font-family=\"monospace\" font-size=\"18\" font-weight=\"bold\">");
    fprintf(svg, "PhenoMemory Token State Visualization</text>\n");
    
    // Generate nodes for each token
    int x_offset = 100;
    int y_offset = 100;
    int node_width = 180;
    int node_height = 120;
    int h_spacing = 220;
    int cols = 4;
    
    for (int i = 0; i < count && i < 12; i++) {
        PhenoToken* token = &tokens[i];
        int col = i % cols;
        int row = i / cols;
        int x = x_offset + col * h_spacing;
        int y = y_offset + row * (node_height + 60);
        
        // Draw node rectangle
        fprintf(svg, "  <rect x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" ", 
                x, y, node_width, node_height);
        fprintf(svg, "fill=\"#e8f4f8\" stroke=\"#2196F3\" stroke-width=\"2\" rx=\"5\"/>\n");
        
        // Token info
        fprintf(svg, "  <text x=\"%d\" y=\"%d\" text-anchor=\"middle\" ",
                x + node_width/2, y + 25);
        fprintf(svg, "font-family=\"monospace\" font-size=\"14\" font-weight=\"bold\">");
        fprintf(svg, "%s</text>\n", token->sentinel);
        
        fprintf(svg, "  <text x=\"%d\" y=\"%d\" text-anchor=\"middle\" ",
                x + node_width/2, y + 45);
        fprintf(svg, "font-family=\"monospace\" font-size=\"12\">");
        fprintf(svg, "ID: 0x%08X</text>\n", token->token_id);
    }
    
    fprintf(svg, "</svg>\n");
    fclose(svg);
}
