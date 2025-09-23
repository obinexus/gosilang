#!/bin/bash

# GosiUML Automated Build Script
# OBINexus MVP - Phenomenological Memory Platform

set -e  # Exit on error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${BLUE}================================================${NC}"
echo -e "${BLUE}   GosiUML Phenomenological Memory Build       ${NC}"
echo -e "${BLUE}   OBINexus Platform v1.0.0                   ${NC}"
echo -e "${BLUE}================================================${NC}"

# Function to print colored messages
log_info() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

log_warn() {
    echo -e "${YELLOW}[WARN]${NC} $1"
}

log_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Create directory structure
create_directories() {
    log_info "Creating directory structure..."
    mkdir -p bin build lib docs images config
    mkdir -p src/core src/cli
    mkdir -p include templates examples
}

# Create necessary header files
create_headers() {
    log_info "Setting up header files..."
    
    # Check if phenomemory_platform.h exists
    if [ ! -f "include/phenomemory_platform.h" ]; then
        log_warn "phenomemory_platform.h not found, creating from artifact..."
        # You would copy the header content here
    fi
    
    # Create cli_parser.h if missing
    if [ ! -f "include/cli_parser.h" ]; then
        cat > include/cli_parser.h << 'EOF'
#ifndef CLI_PARSER_H
#define CLI_PARSER_H

int parse_cli_args(int argc, char* argv[]);
void print_help(const char* prog_name);

#endif // CLI_PARSER_H
EOF
    fi
    
    # Create gosiuml.h if missing
    if [ ! -f "include/gosiuml.h" ]; then
        cat > include/gosiuml.h << 'EOF'
#ifndef GOSIUML_H
#define GOSIUML_H

#include "phenomemory_platform.h"

// GosiUML specific definitions
#define GOSIUML_VERSION "1.0.0"
#define GOSIUML_PLATFORM "OBINexus"

// Export functions
int parse_token_file(const char* filename);
int generate_svg(const char* output_file);

#endif // GOSIUML_H
EOF
    fi
}

# Create stub source files for missing components
create_stubs() {
    log_info "Creating stub implementations..."
    
    # Create pheno_relation.c stub
    if [ ! -f "src/core/pheno_relation.c" ]; then
        cat > src/core/pheno_relation.c << 'EOF'
#include <stdio.h>
#include "phenomemory_platform.h"

// Phenomenological relation management
typedef struct {
    uint32_t source_id;
    uint32_t target_id;
    float weight;
    char relation_type[32];
} PhenoRelation;

void create_relation(PhenoToken* source, PhenoToken* target, const char* type) {
    printf("[RELATION] Creating %s relation between 0x%08X and 0x%08X\n",
           type, source->token_id, target->token_id);
}

void destroy_relation(PhenoRelation* rel) {
    printf("[RELATION] Destroying relation\n");
}
EOF
    fi
    
    # Create token_parser.c stub
    if [ ! -f "src/core/token_parser.c" ]; then
        cat > src/core/token_parser.c << 'EOF'
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gosiuml.h"

int parse_token_file(const char* filename) {
    printf("[PARSER] Parsing token file: %s\n", filename);
    
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        printf("[PARSER] Could not open file: %s\n", filename);
        return -1;
    }
    
    char line[256];
    int token_count = 0;
    
    while (fgets(line, sizeof(line), fp)) {
        // Parse token definitions
        if (strstr(line, "TOKEN:")) {
            token_count++;
            printf("[PARSER] Found token definition #%d\n", token_count);
        }
    }
    
    fclose(fp);
    printf("[PARSER] Parsed %d tokens\n", token_count);
    return token_count;
}
EOF
    fi
    
    # Create svg_generator.c stub
    if [ ! -f "src/core/svg_generator.c" ]; then
        cat > src/core/svg_generator.c << 'EOF'
#include <stdio.h>
#include <stdlib.h>
#include "gosiuml.h"

int generate_svg(const char* output_file) {
    printf("[SVG] Generating state diagram: %s\n", output_file);
    
    FILE* fp = fopen(output_file, "w");
    if (!fp) {
        printf("[SVG] Could not create file: %s\n", output_file);
        return -1;
    }
    
    // Write basic SVG header
    fprintf(fp, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    fprintf(fp, "<svg xmlns=\"http://www.w3.org/2000/svg\" ");
    fprintf(fp, "width=\"800\" height=\"600\">\n");
    
    // Add state machine visualization
    fprintf(fp, "  <rect x=\"50\" y=\"50\" width=\"120\" height=\"60\" ");
    fprintf(fp, "fill=\"#e8f4f8\" stroke=\"#2196F3\" stroke-width=\"2\"/>\n");
    fprintf(fp, "  <text x=\"110\" y=\"85\" text-anchor=\"middle\">NIL</text>\n");
    
    fprintf(fp, "</svg>\n");
    fclose(fp);
    
    printf("[SVG] Generated SVG diagram successfully\n");
    return 0;
}
EOF
    fi
    
    # Create CLI components
    if [ ! -f "src/cli/cli_parser.c" ]; then
        cat > src/cli/cli_parser.c << 'EOF'
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "cli_parser.h"

int parse_cli_args(int argc, char* argv[]) {
    int opt;
    
    while ((opt = getopt(argc, argv, "hvf:o:")) != -1) {
        switch (opt) {
            case 'h':
                print_help(argv[0]);
                return 0;
            case 'v':
                printf("GosiUML v1.0.0\n");
                return 0;
            case 'f':
                printf("Input file: %s\n", optarg);
                break;
            case 'o':
                printf("Output file: %s\n", optarg);
                break;
            default:
                fprintf(stderr, "Unknown option: %c\n", opt);
                return -1;
        }
    }
    return 0;
}

void print_help(const char* prog_name) {
    printf("Usage: %s [options]\n", prog_name);
    printf("Options:\n");
    printf("  -h          Show help\n");
    printf("  -v          Show version\n");
    printf("  -f <file>   Input file\n");
    printf("  -o <file>   Output file\n");
}
EOF
    fi
    
    if [ ! -f "src/cli/main.c" ]; then
        cat > src/cli/main.c << 'EOF'
#include <stdio.h>
#include "cli_parser.h"
#include "gosiuml.h"

int main(int argc, char* argv[]) {
    printf("GosiUML CLI Tool v1.0.0\n");
    printf("OBINexus Phenomenological Memory Platform\n\n");
    
    int result = parse_cli_args(argc, argv);
    if (result != 0) {
        return result;
    }
    
    // Test token parsing
    parse_token_file("config/token_file.txt");
    
    // Generate SVG
    generate_svg("output/state_diagram.svg");
    
    return 0;
}
EOF
    fi
}

# Create example configuration
create_config() {
    log_info "Creating configuration files..."
    
    if [ ! -f "config/token_file.txt" ]; then
        cat > config/token_file.txt << 'EOF'
# GosiUML Token Configuration
# Format: TOKEN: <id> <type> <zone>

TOKEN: 0x10000001 PHENO_NIL 0
TOKEN: 0x10000002 ALLOCATED 1
TOKEN: 0x10000003 LOCKED 2
TOKEN: 0x10000004 ACTIVE 3
TOKEN: 0x10000005 DEGRADED 4

# Relations
RELATION: 0x10000001 -> 0x10000002 : alloc
RELATION: 0x10000002 -> 0x10000003 : lock
RELATION: 0x10000003 -> 0x10000004 : validate
EOF
    fi
}

# Clean build artifacts
clean_build() {
    log_info "Cleaning previous build..."
    make clean 2>/dev/null || true
}

# Build the project
build_project() {
    log_info "Building GosiUML..."
    
    # Check if Makefile exists
    if [ ! -f "Makefile" ]; then
        log_error "Makefile not found!"
        exit 1
    fi
    
    # Build with make
    if make all; then
        log_info "Build successful!"
    else
        log_error "Build failed!"
        exit 1
    fi
}

# Run tests
run_tests() {
    log_info "Running tests..."
    
    if [ -f "bin/gosiuml" ]; then
        ./bin/gosiuml -b
        log_info "Basic test completed"
    else
        log_warn "Test binary not found"
    fi
}

# Display build results
show_results() {
    echo -e "\n${GREEN}=== Build Complete ===${NC}"
    
    if [ -d "bin" ]; then
        echo -e "${BLUE}Binaries:${NC}"
        ls -la bin/ 2>/dev/null | grep -v "^d" | grep -v "^total"
    fi
    
    if [ -d "lib" ]; then
        echo -e "${BLUE}Libraries:${NC}"
        ls -la lib/ 2>/dev/null | grep -v "^d" | grep -v "^total"
    fi
    
    if [ -d "build" ]; then
        echo -e "${BLUE}Object files:${NC}"
        ls -la build/*.o 2>/dev/null | wc -l | xargs echo "  Total:"
    fi
}

# Main execution
main() {
    # Parse command line arguments
    case "${1:-all}" in
        clean)
            clean_build
            ;;
        setup)
            create_directories
            create_headers
            create_stubs
            create_config
            ;;
        build)
            build_project
            ;;
        test)
            run_tests
            ;;
        all)
            create_directories
            create_headers
            create_stubs
            create_config
            clean_build
            build_project
            show_results
            ;;
        *)
            echo "Usage: $0 [clean|setup|build|test|all]"
            exit 1
            ;;
    esac
}

# Run main function
main "$@"

echo -e "\n${GREEN}[DONE]${NC} GosiUML build process complete!"
echo -e "Run ${YELLOW}./bin/gosiuml -h${NC} for usage information"
