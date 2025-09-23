#include <stdio.h>

int main() {
    FILE* svg = fopen("state_machine.svg", "w");
    
    fprintf(svg, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    fprintf(svg, "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"900\" height=\"700\">\n");
    fprintf(svg, "  <title>GosiUML State Machine</title>\n");
    
    // Define arrow marker
    fprintf(svg, "  <defs>\n");
    fprintf(svg, "    <marker id=\"arrow\" markerWidth=\"10\" markerHeight=\"10\" ");
    fprintf(svg, "refX=\"9\" refY=\"3\" orient=\"auto\">\n");
    fprintf(svg, "      <polygon points=\"0,0 9,3 0,6\" fill=\"#666\"/>\n");
    fprintf(svg, "    </marker>\n");
    fprintf(svg, "  </defs>\n");
    
    // Background
    fprintf(svg, "  <rect width=\"900\" height=\"700\" fill=\"#f8f9fa\"/>\n");
    
    // Title
    fprintf(svg, "  <text x=\"450\" y=\"30\" text-anchor=\"middle\" ");
    fprintf(svg, "font-size=\"20\" font-weight=\"bold\">GosiUML Phenomenological State Machine</text>\n");
    
    // State boxes
    // NIL
    fprintf(svg, "  <rect x=\"50\" y=\"80\" width=\"120\" height=\"60\" ");
    fprintf(svg, "fill=\"#e3f2fd\" stroke=\"#1976d2\" stroke-width=\"2\" rx=\"5\"/>\n");
    fprintf(svg, "  <text x=\"110\" y=\"115\" text-anchor=\"middle\">NIL</text>\n");
    
    // ALLOCATED
    fprintf(svg, "  <rect x=\"250\" y=\"80\" width=\"120\" height=\"60\" ");
    fprintf(svg, "fill=\"#e8f5e9\" stroke=\"#4caf50\" stroke-width=\"2\" rx=\"5\"/>\n");
    fprintf(svg, "  <text x=\"310\" y=\"115\" text-anchor=\"middle\">ALLOCATED</text>\n");
    
    // LOCKED
    fprintf(svg, "  <rect x=\"450\" y=\"80\" width=\"120\" height=\"60\" ");
    fprintf(svg, "fill=\"#fff3e0\" stroke=\"#ff9800\" stroke-width=\"2\" rx=\"5\"/>\n");
    fprintf(svg, "  <text x=\"510\" y=\"115\" text-anchor=\"middle\">LOCKED</text>\n");
    
    // ACTIVE
    fprintf(svg, "  <rect x=\"650\" y=\"80\" width=\"120\" height=\"60\" ");
    fprintf(svg, "fill=\"#f3e5f5\" stroke=\"#9c27b0\" stroke-width=\"2\" rx=\"5\"/>\n");
    fprintf(svg, "  <text x=\"710\" y=\"115\" text-anchor=\"middle\">ACTIVE</text>\n");
    
    // DEGRADED
    fprintf(svg, "  <rect x=\"650\" y=\"280\" width=\"120\" height=\"60\" ");
    fprintf(svg, "fill=\"#ffebee\" stroke=\"#f44336\" stroke-width=\"2\" rx=\"5\"/>\n");
    fprintf(svg, "  <text x=\"710\" y=\"315\" text-anchor=\"middle\">DEGRADED</text>\n");
    
    // SHARED
    fprintf(svg, "  <rect x=\"350\" y=\"280\" width=\"120\" height=\"60\" ");
    fprintf(svg, "fill=\"#e0f2f1\" stroke=\"#009688\" stroke-width=\"2\" rx=\"5\"/>\n");
    fprintf(svg, "  <text x=\"410\" y=\"315\" text-anchor=\"middle\">SHARED</text>\n");
    
    // FREED
    fprintf(svg, "  <rect x=\"350\" y=\"480\" width=\"120\" height=\"60\" ");
    fprintf(svg, "fill=\"#eceff1\" stroke=\"#607d8b\" stroke-width=\"2\" rx=\"5\"/>\n");
    fprintf(svg, "  <text x=\"410\" y=\"515\" text-anchor=\"middle\">FREED</text>\n");
    
    // Transitions
    fprintf(svg, "  <!-- Transitions -->\n");
    // NIL -> ALLOCATED
    fprintf(svg, "  <path d=\"M 170 110 L 250 110\" stroke=\"#333\" stroke-width=\"2\" marker-end=\"url(#arrow)\"/>\n");
    fprintf(svg, "  <text x=\"210\" y=\"100\" text-anchor=\"middle\" font-size=\"12\">EVENT_ALLOC</text>\n");
    
    // ALLOCATED -> LOCKED
    fprintf(svg, "  <path d=\"M 370 110 L 450 110\" stroke=\"#333\" stroke-width=\"2\" marker-end=\"url(#arrow)\"/>\n");
    fprintf(svg, "  <text x=\"410\" y=\"100\" text-anchor=\"middle\" font-size=\"12\">EVENT_LOCK</text>\n");
    
    // LOCKED -> ACTIVE
    fprintf(svg, "  <path d=\"M 570 110 L 650 110\" stroke=\"#333\" stroke-width=\"2\" marker-end=\"url(#arrow)\"/>\n");
    fprintf(svg, "  <text x=\"610\" y=\"100\" text-anchor=\"middle\" font-size=\"12\">EVENT_VALIDATE</text>\n");
    
    // ACTIVE -> DEGRADED
    fprintf(svg, "  <path d=\"M 710 140 L 710 280\" stroke=\"#333\" stroke-width=\"2\" marker-end=\"url(#arrow)\"/>\n");
    fprintf(svg, "  <text x=\"750\" y=\"210\" font-size=\"12\">EVENT_DEGRADE</text>\n");
    
    // DEGRADED -> ACTIVE
    fprintf(svg, "  <path d=\"M 650 310 Q 600 210 650 140\" stroke=\"#333\" stroke-width=\"2\" marker-end=\"url(#arrow)\"/>\n");
    fprintf(svg, "  <text x=\"580\" y=\"230\" font-size=\"12\">EVENT_RECOVER</text>\n");
    
    // ACTIVE -> SHARED
    fprintf(svg, "  <path d=\"M 650 120 Q 500 200 470 280\" stroke=\"#333\" stroke-width=\"2\" marker-end=\"url(#arrow)\"/>\n");
    fprintf(svg, "  <text x=\"520\" y=\"200\" font-size=\"12\">EVENT_SHARE</text>\n");
    
    // SHARED -> FREED
    fprintf(svg, "  <path d=\"M 410 340 L 410 480\" stroke=\"#333\" stroke-width=\"2\" marker-end=\"url(#arrow)\"/>\n");
    fprintf(svg, "  <text x=\"450\" y=\"410\" font-size=\"12\">EVENT_FREE</text>\n");
    
    fprintf(svg, "</svg>\n");
    fclose(svg);
    
    printf("Generated: state_machine.svg\n");
    return 0;
}
