#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void main() {
    char opcode[20], operand[20], symbol[20], label[20], code[20], mnemonic[25], character, add[20], objectcode[20], start[20];
    int flag, flag1, locctr, location, loc;
    FILE *fp1, *fp2, *fp3, *fp4, *fp5;

    // Open files
    fp1 = fopen("out3.txt", "r");
    fp2 = fopen("twoout.txt", "w");
    fp3 = fopen("opcode.txt", "r");
    fp4 = fopen("sym1.txt", "r");
    fp5 = fopen("record.txt","w");

    // Check if files opened correctly
    if (!fp1 || !fp2 || !fp3 || !fp4) {
        printf("Error opening files.\n");
        return;
    }

    fscanf(fp1, "%s%s%s", label, opcode, start);

    // If opcode is START, initialize location counter
    if (strcmp(opcode, "START") == 0) {
        fprintf(fp2, "%s\t%s\t%s\n", label, opcode, operand);
        //start=operand;
        fprintf(fp5,"H^%s^%s\n",label,start);
        fprintf(fp5,"T^%s^LEN",start);
        fscanf(fp1, "%d%s%s%s", &locctr, label, opcode, operand);
    }

    // Main assembly loop
    while (strcmp(opcode, "END") != 0) {
        flag = 0;
        rewind(fp3);  // Reset opcode file pointer to the beginning
        fscanf(fp3, "%s%s", code, mnemonic);

        // Search for opcode in opcode table
        while (strcmp(code, "END") != 0) {
            if ((strcmp(opcode, code) == 0) && (strcmp(mnemonic, "*") != 0)) {
                flag = 1;
                break;
            }
            fscanf(fp3, "%s%s", code, mnemonic);
        }

        if (flag == 1) {
            flag1 = 0;
            rewind(fp4);  // Reset symbol file pointer to the beginning

            // Search for operand in symbol table
            while (fscanf(fp4, "%d %s",&loc, symbol) != EOF) {
                if (strcmp(symbol, operand) == 0) {
                    flag1 = 1;
                    break;
                }
            }

            // If symbol found, generate object code
            if (flag1 == 1) {
                sprintf(add, "%04d", loc);  // Zero-padded address for consistency
                strcpy(objectcode, strcat(mnemonic, add));
            } else {
                strcpy(objectcode, "0000");  // If symbol not found, set object code as undefined
            }
        } 
        else if (strcmp(opcode, "BYTE") == 0 || strcmp(opcode, "WORD") == 0) {
            if (operand[0] == 'C') {
                // Convert character to ASCII
                character = operand[2];  // Assume single character for simplicity
                sprintf(objectcode, "%06X", character);
            } 
            else if (operand[0] == 'X') {
                // Direct hex conversion
                strncpy(objectcode, operand + 2, strlen(operand) - 3);
                objectcode[strlen(operand) - 3] = '\0';
            } 
            else {
                sprintf(objectcode, "%06d", atoi(operand));  // WORD as a decimal value with padding
            }
        } 
        else {
            strcpy(objectcode, "");  // If no match, set object code to empty
        }

        // Output line with current information
        fprintf(fp2, "%s\t%s\t%s\t%d\t%s\n", label, opcode, operand, locctr, objectcode);
        fprintf(fp5,"^%s",objectcode);

        // Read next line from input file
        fscanf(fp1, "%d%s%s%s", &locctr, label, opcode, operand);
    }

    // Final END line
    fprintf(fp2, "%s\t%s\t%s\t%d\n", label, opcode, operand, locctr);
    fprintf(fp5,"\nE^%s",start);

    // Close all files
    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
    fclose(fp4);
    fclose(fp5);
}
