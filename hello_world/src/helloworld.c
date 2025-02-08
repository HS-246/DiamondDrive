#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>  // For atoi()
#include <string.h>
#include "platform.h"
#include "xil_printf.h"
#include "xuartlite.h"
#include "key-expansion.h"
#include "encryption.h"
#include "decryption.h"

#define UARTLITE_DEVICE_ID 0   // UART Lite device ID
#define MAX_USERS 10
#define USERNAME_SIZE 32
#define PASSWORD_SIZE 16
#define N 4
#define WEBSITE_SIZE 64
#define AES_KEY_SIZE 16  // 128-bit key

XUartLite UartLite;  /* Instance of the UartLite device */

/* Function prototypes */
int UartLiteSelfTestExample(u16 DeviceId);
void echo_input(u8 *text, int length);
int get_number_from_uart();
void freeMatrix(uint8_t** matrix);
void get_string_from_uart(char *input, int max_length);
void xor_encrypt_decrypt(char *input, char *output, char *key, int key_len);
void add_user();
void retrieve_password();
void show_all_users();


/* User structure to store username, website, and encrypted password */
typedef struct {
    char username[USERNAME_SIZE];
    char website[WEBSITE_SIZE];
    uint8_t **encrypted_password;
} User;

User password_db[MAX_USERS];  // Database of users
int user_count = 0;           // Track number of stored users

// Fixed 128-bit XOR encryption key (AES-like key)
//char xor_key[] = "1234567890abcdef";  // 128-bit key for XOR encryption
//int xor_key_len = AES_KEY_SIZE; // Key length (16 bytes)

int main()
{
    init_platform();

    int UART_Status;

    // Perform UART self-test and initialization
    UART_Status = UartLiteSelfTestExample(UARTLITE_DEVICE_ID);
    if (UART_Status != XST_SUCCESS) {
        xil_printf("\033[31mUartlite Setup Failed!\033[0m\r\n");
        return -1;
    } else {
        xil_printf("\033[32mUartlite Setup Successful!\033[0m\r\n");
        xil_printf("\n");
    }

    xil_printf("\033[1;35m---------------------------------------------------------\033[0m\n\r");
    xil_printf("\033[1;36m        Welcome to AES 128-bit Password Manager\033[0m\n\r");
    xil_printf("\033[1;35m---------------------------------------------------------\033[0m\n\r");

    while (1) {
        xil_printf("\n\033[32m---------------------------------------------------------\033[0m\n\r");
        xil_printf("\033[34mChoose an option:\033[0m\r\n");
        xil_printf("1 - \033[1;33mEnter new user details (with encryption)\033[0m\r\n");
        xil_printf("2 - \033[1;33mRetrieve user password (decryption)\033[0m\r\n");
        xil_printf("3 - \033[1;33mShow all users' data\033[0m\r\n");
        xil_printf("4 - \033[1;31mExit\033[0m\r\n");

        int choice = get_number_from_uart();
        xil_printf("\033[35mOption selected by you is: \033[1;37m%d\033[0m\r\n", choice);

        switch (choice) {
            case 1:
                add_user();
                break;
            case 2:
                retrieve_password();
                break;
            case 3:
                show_all_users();
                break;
            case 4:
                xil_printf("\033[32mExiting... Goodbye!\033[0m\r\n");
                cleanup_platform();
                return 0;
            default:
                xil_printf("\033[31mInvalid choice, try again.\033[0m\r\n");
        }
    }

    cleanup_platform();
    return 0;
}


/* Add a new user and store their details with encryption */
void add_user() {
    xil_printf("\033[32mEnter username: \033[0m");
    char username[USERNAME_SIZE];
    get_string_from_uart(username, USERNAME_SIZE);

    xil_printf("\033[32mEnter website (e.g., google.com): \033[0m");
    char website[WEBSITE_SIZE];
    get_string_from_uart(website, WEBSITE_SIZE);

    xil_printf("\033[32mEnter password: \033[0m");
    char password[PASSWORD_SIZE];
    get_string_from_uart(password, PASSWORD_SIZE);

    //xil_printf("Right after taking: %s\n\n",password);
    // Encrypt the password
    // uint8_t** encrypted_password;
    // encrypted_password=encryptAES(password);

    // Store the user in the database
    if (user_count < MAX_USERS) {
        //xil_printf("entered addditiion to db");
        strcpy(password_db[user_count].username, username);
        strcpy(password_db[user_count].website, website);
        password_db[user_count].encrypted_password=encryptAES(password);
        // xil_printf("finished addition");
        // xil_printf("After encryption function: \n");
    
        // for (int i = 0; i < 4; i++)
        // {
        //     for (int j = 0; j < 4; j++)
        //     {
        //         xil_printf("%x ",(password_db[user_count].encrypted_password)[j][i]);
        //     }
        // }
        user_count++;
        xil_printf("\033[32mUser details added and password encrypted successfully!\033[0m\r\n");
        //free(encrypted_password);
    } else {
        xil_printf("\033[31mUser database full, cannot add more users.\033[0m\r\n");
    }
}

/* Retrieve and decrypt the password for a user */
void retrieve_password() {
    xil_printf("\033[32mEnter username: \033[0m");
    char username[USERNAME_SIZE];
    get_string_from_uart(username, USERNAME_SIZE);

    // Search for the user in the database
    for (int i = 0; i < user_count; i++) {
        if (strcmp(password_db[i].username, username) == 0) {
            // Decrypt the password
            // xil_printf("user Found!");

            // xil_printf("encrypted password:");
    
            // for (int i = 0; i < 4; i++)
            // {
            //     for (int j = 0; j < 4; j++)
            //     {
            //         xil_printf("%x ",password_db[user_count].encrypted_password[j][i]);
            //     }
            // }

            uint8_t** decrypted_password=decryptAES(password_db[i].encrypted_password);
            int index=0;
            char decrypted_string[17]="";
            for(int i=0;i<4;i++){
                for(int j=0;j<4;j++){
                    if(decrypted_password[j][i]!=0x00){
                        decrypted_string[index++]=(char)decrypted_password[j][i];
                    }
                }                
            }
            decrypted_string[index]='\0';

            // xil_printf("Decrypted password: %s",decrypted_string);
            //decrypted_password=decryptAES(password_db[i].encrypted_password);

            xil_printf("\033[32mPassword for user '%s' at website '%s': %s\033[0m\n\n", username, password_db[i].website,decrypted_string);
            // xil_printf("%s\n", decrypted_password);
            freeMatrix(decrypted_password);
            return;
        }
    }

    xil_printf("\033[31mUser not found.\033[0m\r\n");
}

/* Show all users stored in the database */
void show_all_users() {
    if (user_count == 0) {
        xil_printf("\033[31mNo users stored in the database.\033[0m\r\n");
        return;
    }

    xil_printf("\033[32mStored Users:\033[0m\r\n");
    for (int i = 0; i < user_count; i++) {
        xil_printf("\033[1;33m%d. \033[1;37mUsername: \033[0m%s, \033[1;37mWebsite: \033[0m%s,\n \033[1;37mEncrypted Password: \033[0m\n", i + 1, password_db[i].username, password_db[i].website);
        
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                xil_printf("%x ",(password_db[user_count].encrypted_password)[j][i]);
            }
        }
    }
}

/* Get a number input (used for menu options) */
int get_number_from_uart() {
    u8 text[10];
    int received_count = 0;
    while (1) {
        int byte_count = XUartLite_Recv(&UartLite, &text[received_count], 1);
        if (byte_count > 0) {
            if (text[received_count] == '\r' || text[received_count] == '\n') {
                text[received_count] = '\0';
                break;
            }
            received_count++;
        }
    }
    return atoi((char *)text);
}

/* Get a string input (with backspace support) */
void get_string_from_uart(char *input, int max_length) {
    int index = 0;
    xil_printf("\n");

    while (index < max_length - 1) {
        int byte_count = XUartLite_Recv(&UartLite, (u8 *)&input[index], 1);
        if (byte_count > 0) {
            if (input[index] == '\r' || input[index] == '\n') {
                input[index] = '\0';  // Null-terminate the string
                xil_printf("\r\n");
                break;
            } else if (input[index] == 0x08 && index > 0) {  // Backspace
                xil_printf("\b \b");  // Move cursor back, print space, move back again
                index--;
            } else {
                xil_printf("%c", input[index]);  // Echo the character typed
                index++;
            }
        }
    }
    input[index] = '\0';  // Ensure null termination
}

/* Echo the input received */
void echo_input(u8 *text, int length) {
    XUartLite_Send(&UartLite, text, length);
}

/* Perform UART self-test and initialization */
int UartLiteSelfTestExample(u16 DeviceId) {
    int Status;

    Status = XUartLite_Initialize(&UartLite, DeviceId);
    if (Status != XST_SUCCESS) {
        xil_printf("Failed to initialize UART!\r\n");
        return XST_FAILURE;
    }

    Status = XUartLite_SelfTest(&UartLite);
    if (Status != XST_SUCCESS) {
        xil_printf("UART self-test failed!\r\n");
        return XST_FAILURE;
    }

    xil_printf("UART initialization and self-test passed.\r\n");
    return XST_SUCCESS;
}

void freeMatrix(uint8_t** matrix) {
    for (int i = 0; i < 4; i++) {
        free(matrix[i]); // Free each row
    }
    free(matrix); // Free the top-level pointer
}