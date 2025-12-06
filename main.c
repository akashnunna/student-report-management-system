#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#define STUDENT_FILE "student.txt"
#define CREDENTIAL_FILE "credentials.txt"

struct student {
    char roll[20];
    char name[50];
    float marks;
};

char currentRole[10];
char currentUser[50];

// Function declarations
int loginsystem();
void mainMenu();
void adminMenu();
void userMenu();
void staffMenu();
void guestMenu();
void addStudent();
void displayStudents();
void searchStudent();
void updateStudent();
void deleteStudent();

// -------------------- VALIDATION FUNCTIONS --------------------

int isAlphaOnly(char *str) {
    for (int i = 0; str[i]; i++)
        if (!((str[i] >= 'A' && str[i] <= 'Z') ||
              (str[i] >= 'a' && str[i] <= 'z')))
            return 0;
    return 1;
}

int isAlnumOnly(char *str) {
    for (int i = 0; str[i]; i++)
        if (!((str[i] >= 'A' && str[i] <= 'Z') ||
              (str[i] >= 'a' && str[i] <= 'z') ||
              (str[i] >= '0' && str[i] <= '9')))
            return 0;
    return 1;
}

int isFloatNumber(char *str) {
    int dot = 0;
    for (int i = 0; str[i]; i++) {
        if (str[i] == '.') {
            dot++;
            if (dot > 1) return 0;
        }
        else if (!(str[i] >= '0' && str[i] <= '9')) {
            return 0;
        }
    }
    return 1;
}

void readLine(char *buf, int size) {
    while (1) {
        if (!fgets(buf, size, stdin)) {
            buf[0] = '\0';
            return;
        }
        buf[strcspn(buf, "\n")] = 0;
        if (buf[0] != '\0') return;
    }
}

int getIntInput() {
    char buf[100];
    while (1) {
        readLine(buf, sizeof(buf));
        if (isFloatNumber(buf)) return atoi(buf);
        printf("Invalid number! Enter again: ");
    }
}


// -------------------- LOGIN SYSTEM --------------------

int loginsystem() {
    char username[50], password[50];
    char fileUser[50], filePass[50], fileRole[10];
    int attempts = 3;

    while (attempts > 0) {
        printf("\n====== LOGIN ======\n");
        printf("Username: ");
        readLine(username, sizeof(username));

        printf("Password: ");
        int index = 0;
        memset(password, 0, sizeof(password));
        char ch;
        while ((ch = getch()) != '\r') {
            if (ch == '\b' && index > 0) {
                index--;
                printf("\b \b");
            } else if (ch != '\b') {
                password[index++] = ch;
                printf("*");
            }
        }
        password[index] = '\0';
        printf("\n");

        FILE *fp = fopen(CREDENTIAL_FILE, "r");
        if (!fp) return 0;

        int ok = 0;
        while (fscanf(fp, "%49s %49s %9s",
                      fileUser, filePass, fileRole) == 3) {
            if (strcmp(username, fileUser) == 0 &&
                strcmp(password, filePass) == 0) {
                strcpy(currentRole, fileRole);
                strcpy(currentUser, fileUser);
                ok = 1;
                break;
            }
        }
        fclose(fp);

        if (ok) {
            printf("\nLogin Successful. Welcome %s (%s)\n",
                   currentUser, currentRole);
            return 1;
        } else {
            attempts--;
            printf("Incorrect credentials. Attempts left: %d\n",
                   attempts);
        }
    }
    return 0;
}


// -------------------- MAIN MENU --------------------

void mainMenu() {
    if (strcmp(currentRole, "ADMIN") == 0) adminMenu();
    else if (strcmp(currentRole, "USER") == 0) userMenu();
    else if (strcmp(currentRole, "STAFF") == 0) staffMenu();
    else guestMenu();
}


// -------------------- ADMIN MENU --------------------

void adminMenu() {
    int choice;

    while (1) {
        printf("\n===== ADMIN MENU =====\n");
        printf("1. Add Student\n");
        printf("2. Display Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. Logout\n");
        printf("Enter choice: ");
        choice = getIntInput();

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: return;
            default: printf("Invalid choice\n");
        }
    }
}


// -------------------- ADD STUDENT --------------------

void addStudent() {
    struct student s;
    char buffer[100];
    FILE *fp = fopen(STUDENT_FILE, "a");

    printf("Enter Roll: ");
    while (1) {
        readLine(buffer, sizeof(buffer));
        if (isAlnumOnly(buffer)) break;
        printf("Invalid Roll! Letters and digits only: ");
    }
    strcpy(s.roll, buffer);

    printf("Enter Name: ");
    while (1) {
        readLine(buffer, sizeof(buffer));
        if (isAlphaOnly(buffer)) break;
        printf("Invalid Name! Alphabets only: ");
    }
    strcpy(s.name, buffer);

    printf("Enter Marks (float allowed): ");
    while (1) {
        readLine(buffer, sizeof(buffer));
        if (isFloatNumber(buffer)) {
            s.marks = atof(buffer);
            break;
        }
        printf("Invalid Marks! Enter valid number: ");
    }

    fprintf(fp, "%s %s %.2f\n", s.roll, s.name, s.marks);
    fclose(fp);

    printf("Student Added Successfully.\n");
}


// -------------------- DISPLAY STUDENTS --------------------

void displayStudents() {
    struct student s;
    FILE *fp = fopen(STUDENT_FILE, "r");

    if (!fp) {
        printf("No Records Found.\n");
        return;
    }

    printf("\n--- Student List ---\n");
    while (fscanf(fp, "%19s %49s %f",
                  s.roll, s.name, &s.marks) == 3)
        printf("Roll: %s | Name: %s | Marks: %.2f\n",
               s.roll, s.name, s.marks);

    fclose(fp);
}


// -------------------- SEARCH STUDENT --------------------

void searchStudent() {
    char r[20];
    struct student s;
    int found = 0;

    printf("Enter Roll to Search: ");
    readLine(r, sizeof(r));

    FILE *fp = fopen(STUDENT_FILE, "r");
    if (!fp) return;

    while (fscanf(fp, "%19s %49s %f",
                  s.roll, s.name, &s.marks) == 3) {
        if (strcmp(s.roll, r) == 0) {
            printf("Found: Roll=%s Name=%s Marks=%.2f\n",
                   s.roll, s.name, s.marks);
            found = 1;
            break;
        }
    }

    fclose(fp);
    if (!found) printf("Student Not Found.\n");
}


// -------------------- UPDATE STUDENT --------------------

void updateStudent() {
    char r[20], buffer[100];
    struct student s;
    int found = 0;

    printf("Enter Roll to Update: ");
    readLine(r, sizeof(r));

    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");

    if (!fp || !temp) {
        printf("File Error!\n");
        return;
    }

    while (fscanf(fp, "%19s %49s %f",
                  s.roll, s.name, &s.marks) == 3) {

        if (strcmp(s.roll, r) == 0) {
            found = 1;

            printf("Enter New Name: ");
            while (1) {
                readLine(buffer, sizeof(buffer));
                if (isAlphaOnly(buffer)) break;
                printf("Invalid Name! Alphabets only: ");
            }
            strcpy(s.name, buffer);

            printf("Enter New Marks (float allowed): ");
            while (1) {
                readLine(buffer, sizeof(buffer));
                if (isFloatNumber(buffer)) {
                    s.marks = atof(buffer);
                    break;
                }
                printf("Invalid Marks! Enter valid number: ");
            }
        }

        fprintf(temp, "%s %s %.2f\n",
                s.roll, s.name, s.marks);
    }

    fclose(fp);
    fclose(temp);

    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);

    if (found) printf("Record Updated Successfully.\n");
    else printf("Student Not Found.\n");
}


// -------------------- DELETE STUDENT --------------------

void deleteStudent() {
    char r[20];
    struct student s;
    int found = 0;

    printf("Enter Roll to Delete: ");
    readLine(r, sizeof(r));

    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");

    while (fscanf(fp, "%19s %49s %f",
                  s.roll, s.name, &s.marks) == 3) {
        if (strcmp(s.roll, r) != 0)
            fprintf(temp, "%s %s %.2f\n",
                    s.roll, s.name, s.marks);
        else found = 1;
    }

    fclose(fp);
    fclose(temp);

    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);

    if (found) printf("Record Deleted Successfully.\n");
    else printf("Student Not Found.\n");
}


// -------------------- ROLE MENUS --------------------

void userMenu() {
    printf("\n===== USER MENU =====\n");
    displayStudents();
}

void staffMenu() {
    printf("\n===== STAFF MENU =====\n");
    searchStudent();
}

void guestMenu() {
    printf("\n===== GUEST MENU =====\n");
    displayStudents();
}


// -------------------- MAIN --------------------

int main() {
    if (loginsystem()) 
        mainMenu();
    else 
        printf("Access Denied.\n");

    return 0;
}
