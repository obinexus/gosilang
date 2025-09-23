#include <stdio.h>
#include <stdlib.h>
#include "phenomemory_platform.h"

void generate_state_machine_svg(const char* filename) {
    FILE* fp = fopen(filename, "w");
    if (!fp) return;
    
    // SVG header
    fprintf(fp, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    fprintf(fp, "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"800\" height=\"600\">\n");
    
    // States
    fprintf(fp, "  <!-- States -->\n");
    fprintf(fp, "  <rect x=\"50\" y=\"50\" width=\"120\" height=\"60\" fill=\"#e8f4f8\" stroke=\"#2196F3\" stroke-width=\"2\" rx=\"5\"/>\n");
    fprintf(fp, "  <text x=\"110\" y=\"85\" text-anchor=\"middle\" font-family=\"monospace\">NIL</text>\n");
    
    fprintf(fp, "  <rect x=\"250\" y=\"50\" width=\"120\" height=\"60\" fill=\"#e8f4f8\" stroke=\"#2196F3\" stroke-width=\"2\" rx=\"5\"/>\n");
    fprintf(fp, "  <text x=\"310\" y=\"85\" text-anchor=\"middle\" font-family=\"monospace\">ALLOCATED</text>\n");
    
    fprintf(fp, "  <rect x=\"450\" y=\"50\" width=\"120\" height=\"60\" fill=\"#e8f4f8\" stroke=\"#2196F3\" stroke-width=\"2\" rx=\"5\"/>\n");
    fprintf(fp, "  <text x=\"510\" y=\"85\" text-anchor=\"middle\" font-family=\"monospace\">LOCKED</text>\n");
    
    fprintf(fp, "  <rect x=\"350\" y=\"200\" width=\"120\" height=\"60\" fill=\"#c8e6c9\" stroke=\"#4CAF50\" stroke-width=\"2\" rx=\"5\"/>\n");
    fprintf(fp, "  <text x=\"410\" y=\"235\" text-anchor=\"middle\" font-family=\"monospace\">ACTIVE</text>\n");
    
    fprintf(fp, "  <rect x=\"550\" y=\"350\" width=\"120\" height=\"60\" fill=\"#ffccbc\" stroke=\"#FF5722\" stroke-width=\"2\" rx=\"5\"/>\n");
    fprintf(fp, "  <text x=\"610\" y=\"385\" text-anchor=\"middle\" font-family=\"monospace\">DEGRADED</text>\n");
    
    // Transitions
    fprintf(fp, "  <!-- Transitions -->\n");
    fprintf(fp, "  <path d=\"M 170 80 L 250 80\" stroke=\"#666\" stroke-width=\"2\" marker-end=\"url(#arrow)\"/>\n");
    fprintf(fp, "  <text x=\"210\" y=\"70\" font-family=\"monospace\" font-size=\"10\">alloc()</text>\n");
    
    fprintf(fp, "  <path d=\"M 370 80 L 450 80\" stroke=\"#666\" stroke-width=\"2\" marker-end=\"url(#arrow)\"/>\n");
    fprintf(fp, "  <text x=\"410\" y=\"70\" font-family=\"monospace\" font-size=\"10\">lock()</text>\n");
    
    fprintf(fp, "  <path d=\"M 510 110 L 410 200\" stroke=\"#666\" stroke-width=\"2\" marker-end=\"url(#arrow)\"/>\n");
    fprintf(fp, "  <text x=\"460\" y=\"155\" font-family=\"monospace\" font-size=\"10\">validate()</text>\n");
    
    // Arrow marker
    fprintf(fp, "  <defs>\n");
    fprintf(fp, "    <marker id=\"arrow\" markerWidth=\"10\" markerHeight=\"10\" refX=\"9\" refY=\"3\" orient=\"auto\">\n");
    fprintf(fp, "      <polygon points=\"0,0 9,3 0,6\" fill=\"#666\"/>\n");
    fprintf(fp, "    </marker>\n");
    fprintf(fp, "  </defs>\n");
    
    fprintf(fp, "</svg>\n");
    fclose(fp);
    printf("Generated: %s\n", filename);
}

int main() {
    generate_state_machine_svg("state_machine_diagram.svg");
    printf("View with: firefox state_machine_diagram.svg\n");
    return 0;
}
