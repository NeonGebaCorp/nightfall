#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "nightfall.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_COLOR_LIGHT_GREY 7
#define VGA_COLOR_BLACK 0

static uint16_t* const VGA_BUFFER = (uint16_t*)0xB8000;
static uint8_t terminal_color = VGA_COLOR_LIGHT_GREY;
static uint16_t terminal_row = 0;
static uint16_t terminal_column = 0;

static inline uint8_t vga_entry_color(uint8_t fg, uint8_t bg) {
    return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
    return (uint16_t) uc | (uint16_t) color << 8;
}

void terminal_initialize() {
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            VGA_BUFFER[index] = vga_entry(' ', terminal_color);
        }
    }
}

void terminal_setcolor(uint8_t color) {
    terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
    const size_t index = y * VGA_WIDTH + x;
    VGA_BUFFER[index] = vga_entry(c, color);
}

void terminal_putchar(char c) {
    if (c == '\n') {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT)
            terminal_row = 0;
    } else {
        terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
        if (++terminal_column == VGA_WIDTH) {
            terminal_column = 0;
            if (++terminal_row == VGA_HEIGHT)
                terminal_row = 0;
        }
    }
}

void terminal_writestring(const char* data) {
    size_t datalen = strlen(data);
    for (size_t i = 0; i < datalen; i++)
        terminal_putchar(data[i]);
}

// Implementation of functions
void aprint(const char *message) {
    terminal_writestring(message);
    terminal_putchar('\n');
}

void areboot() {
    asm volatile ("int $0x19"); // BIOS interrupt for reboot
}

void apoweroff() {
    asm volatile (
        "movw $0x2000, %ax\n"
        "movw %ax, %dx\n"
        "outw %dx, $0x604"
    ); // ACPI power off command
}

void abootable() {
    // Setting up bootable signature
    uint8_t boot_sector[512] = {0};
    boot_sector[510] = 0x55;
    boot_sector[511] = 0xAA;

    FILE *fp = fopen("boot.img", "wb");
    if (fp == NULL) {
        perror("Failed to create boot image");
        return;
    }
    fwrite(boot_sector, sizeof(boot_sector), 1, fp);
    fclose(fp);

    printf("Bootable signature set in boot.img\n");
}

void asetgraphics(int width, int height) {
    // This example uses VGA text mode, which has fixed dimensions (80x25)
    if (width == VGA_WIDTH && height == VGA_HEIGHT) {
        terminal_initialize();
        printf("VGA text mode set to %dx%d.\n", width, height);
    } else {
        printf("Invalid graphics mode dimensions: %dx%d. VGA text mode supports only 80x25.\n", width, height);
    }
}

void acolor(const char *hex) {
    // Example of parsing the hex color code (limited to 16 VGA colors)
    if (strcmp(hex, "#000000") == 0) terminal_setcolor(vga_entry_color(VGA_COLOR_BLACK, VGA_COLOR_BLACK));
    else if (strcmp(hex, "#808080") == 0) terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));
    // Add other color mappings as needed
    else printf("Invalid color format or unsupported color: %s\n", hex);
}

void acli() {
    asm volatile ("cli"); // Clear interrupt flag
}

void initfs() {
    // Example of initializing a simple filesystem
    printf("Initializing filesystem.\n");

    FILE *fp = fopen("filesystem.img", "wb");
    if (fp == NULL) {
        perror("Failed to create filesystem image");
        return;
    }

    // Write a simple filesystem structure (for demonstration purposes)
    const char *fs_data = "SimpleFS";
    fwrite(fs_data, strlen(fs_data), 1, fp);
    fclose(fp);

    printf("Filesystem initialized in filesystem.img\n");
}
