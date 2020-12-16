#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#define MAX_PATIENTS 10000
#define MAX_IMAGES 10
#define MAX_IMAGE_LENGTH 10
#define NAME_LENGTH 25
#define SSNUMBER_LENGTH 11
#define FILE_LENGTH 20

struct patientInfo
{
    char ssNumber[SSNUMBER_LENGTH+1]; //social security number
    char name[NAME_LENGTH];           //first and last name
    int imageReferences[MAX_IMAGES];
};

void initialise(char fileGiven[], int *nrOfPatients, struct patientInfo patient[]);
void database_menu(bool *flag, int *nrOfPatients, struct patientInfo patient[]);
void register_patient(int *nrOfPatients, struct patientInfo patient[]);
void enter_ssNumber(int *nrOfPatients, int n, struct patientInfo patient[]);
int check_ssNumber(int *nrOfPatients, struct patientInfo patient[]);
void enter_name(int *nrOfPatients, int n, struct patientInfo patient[]);
void enter_image_references(int *nrOfPatients, int n, struct patientInfo patient[]);
void fill_image_reference_with_zeroes(int *nrOfPatients, struct patientInfo patient[]);
void print_patient_list(int *nrOfPatients, struct patientInfo patient[]);
void print_patient_info(int i, struct patientInfo patient[]);
int search_for_patients(int *nrOfPatients, struct patientInfo patient[]);
int search_ssNumber(int *nrOfPatients, struct patientInfo patient[]);
int search_name(int *nrOfPatients, struct patientInfo patient[]);
int search_image_reference(int *nrOfPatients, struct patientInfo patient[]);
void change_patient_data(int *nrOfPatients, struct patientInfo patient[]);
void sort_patients(int *nrOfPatients, struct patientInfo patient[]);
void sort_by_number(int *nrOfPatients, struct patientInfo patient[]);
void sort_by_name(int *nrOfPatients, struct patientInfo patient[]);
void deregister_patient(int *nrOfPatients, struct patientInfo patient[]);
void quit(bool *flag);
void end_program(char fileGiven[], int *nrOfPatients, struct patientInfo patient[]);


int main()
{
    bool flag=1;
    int nrOfPatients=0;
    struct patientInfo patient[MAX_PATIENTS];
    char fileGiven[FILE_LENGTH+1];
    initialise(fileGiven, &nrOfPatients, patient);
    while(flag)
    {
        database_menu(&flag, &nrOfPatients, patient);
    }
    end_program(fileGiven, &nrOfPatients, patient);
	return 0;
}

void initialise(char fileGiven[], int *nrOfPatients, struct patientInfo patient[])
{
    char space[NAME_LENGTH+1];
    char nameTmp[NAME_LENGTH+1];
    int storeZero;
    char addtxt[FILE_LENGTH+1];
    
    printf("Ange filen du vill anv\x84nda: ");
    fgets(fileGiven, FILE_LENGTH+1, stdin);
    for(int i=0;i<FILE_LENGTH+1;i++)
    {
        if(fileGiven[i]=='\n')
        {
            fileGiven[i] = '\0';
        }
    }
    if(strlen(fileGiven)==FILE_LENGTH)
    {
        while((getchar()) != '\n');
    }
    strcpy(addtxt, ".txt");
    strcat(fileGiven, addtxt);
    strcpy(space, " ");
    
    FILE *fp;
    fp = fopen(fileGiven, "r");
    if(fp!=NULL)
    {
        fscanf(fp, "%d", *&nrOfPatients);
        if(*nrOfPatients<MAX_PATIENTS)
        {
            for(int i=0;i<*nrOfPatients;i++)
            {
                fscanf(fp, "%s", patient[i].ssNumber);
                fscanf(fp, "%s", patient[i].name);
                fscanf(fp, "%s", nameTmp);
                strcat(patient[i].name, space);
                strcat(patient[i].name, nameTmp);
                for(int j=0;j<MAX_IMAGES;j++)
                {
                    fscanf(fp, "%d", &patient[i].imageReferences[j]);
                    if(patient[i].imageReferences[j]==0)
                    {
                        break;
                    }
                }
            }
        }
        fclose(fp);
    }
    else
    {
        printf("Filen fanns inte. Programmet b\x94rjar med en tomm fil.\n");
    }
}

void database_menu(bool *flag, int *nrOfPatients, struct patientInfo patient[])
{
    int option;
    printf("\n1 Registrera nya patienter\n2 Skriv ut alla patienter\n3 S\x94k efter patienter\n");
    printf("4 L\x84gg till bildreferens\n5 Sortera patienter\n6 Avregistrera patienter\n7 Avsluta programmet\n");
    printf("\nVad vill du g\x94ra? ");
    scanf("%d", &option);
    while((getchar()) != '\n');
    
    switch(option)
    {
        case 1: register_patient(*&nrOfPatients, patient);
        break;
        case 2: print_patient_list(*&nrOfPatients, patient);
        break;
        case 3: search_for_patients(*&nrOfPatients, patient);
        break;
        case 4: change_patient_data(*&nrOfPatients, patient);
        break;
        case 5: sort_patients(*&nrOfPatients, patient);
        break;
        case 6: deregister_patient(*&nrOfPatients, patient);
        break;
        case 7: quit(*&flag);
        break;
        default: printf("V\x84lj en av alternativen\n");
    }
}

void register_patient(int *nrOfPatients, struct patientInfo patient[])
{
    int ssNumberOK;
    char endRegistration;
    while(1)
    {
        if((*nrOfPatients)<MAX_PATIENTS)
        {
            enter_ssNumber(*&nrOfPatients, SSNUMBER_LENGTH+1, patient);
            ssNumberOK = check_ssNumber(*&nrOfPatients, patient);
            if(ssNumberOK==0)
            {
                printf("Personnumret \x84r redan registrerat och registrationen kan d\x84rf\x94r inte fort\x84tta\n");
                break;
            }
            enter_name(*&nrOfPatients, NAME_LENGTH+1, patient);
            
            enter_image_references(*&nrOfPatients, MAX_IMAGES, patient);
            
            (*nrOfPatients)++;
            
            printf("Vill du registrera en till patient? (y/n): ");
            scanf(" %c", &endRegistration);
            while((getchar()) != '\n');
            if(endRegistration == 'n' || endRegistration == 'N')
            {
                break;
            }
        }
        else
        {
            printf("Det finns ingen mer plats f\x94r att registrera fler patienter\n");
            break;
        }
    }
}

void enter_ssNumber(int *nrOfPatients, int n, struct patientInfo patient[])
{
    printf("Ange personnummer: ");
    fgets(patient[*nrOfPatients].ssNumber, SSNUMBER_LENGTH+1, stdin);
    for(int i=0;i<SSNUMBER_LENGTH+1;i++)
    {
        if(patient[*nrOfPatients].ssNumber[i]=='\n')
        {
            patient[*nrOfPatients].ssNumber[i] = '\0';
        }
    }
    if(strlen(patient[*nrOfPatients].ssNumber)==SSNUMBER_LENGTH)
    {
        while((getchar()) != '\n');
    }
}

int check_ssNumber(int *nrOfPatients, struct patientInfo patient[])
{
    int i=0, comparison=1;
    while(i<(*nrOfPatients))
    {
        comparison = strcmp(patient[*nrOfPatients].ssNumber,patient[i].ssNumber);
        if(comparison==0)
        {
            break;
        }
        i++;
    }
    return comparison;
}

void enter_name(int *nrOfPatients, int n, struct patientInfo patient[])
{
    printf("Ange fullst\x84ndigt namn: ");
    fgets(patient[*nrOfPatients].name, NAME_LENGTH+1, stdin);
    for(int i=0;i<n;i++)
    {
        if(patient[*nrOfPatients].name[i]=='\n')
        {
            patient[*nrOfPatients].name[i] = '\0';
        }
    }
}

void enter_image_references(int *nrOfPatients, int n, struct patientInfo patient[])
{
    fill_image_reference_with_zeroes(*&nrOfPatients, patient);  //Assumes that there is no image reference labeled 0
    int i=0;
    char imageString[MAX_IMAGE_LENGTH+1];
    char endImageRegistration;
    while(i<MAX_IMAGES)
    {
        printf("Ange bildreferens: ");
        fgets(imageString, MAX_IMAGE_LENGTH+1, stdin);
        patient[*nrOfPatients].imageReferences[i] = atoi(imageString);
        i++;
        printf("Ange en till bildreferens? (y/n): ");
        scanf("%c", &endImageRegistration);
        while((getchar()) != '\n');
        if(endImageRegistration == 'n' || endImageRegistration == 'N')
        {
            break;
        }
    }
}

void fill_image_reference_with_zeroes(int *nrOfPatients, struct patientInfo patient[])
{
    for(int i=0;i<MAX_IMAGES;i++)
    {
        patient[*nrOfPatients].imageReferences[i] = 0;
    }
}

void print_patient_list(int *nrOfPatients, struct patientInfo patient[])
{
    printf("\nPersonnummer             Namn                     Bildreferenser\n");
    printf("_______________________________________________________________\n");
    for(int i=0;i<*nrOfPatients;i++)
    {
        print_patient_info(i, patient);
    }
}

void print_patient_info(int i, struct patientInfo patient[])
{
    printf("%-24s %-25s", patient[i].ssNumber, patient[i].name);
    int j=0;
    printf("[");
    while(j<MAX_IMAGES && patient[i].imageReferences[j] != 0)
    {
        if(patient[i].imageReferences[j+1] == 0 || j == MAX_IMAGES-1)
        {
            printf("%d", patient[i].imageReferences[j]);
            break;
        }
        else
        {
            printf("%d,", patient[i].imageReferences[j]);
            j++;
        }
    }
    printf("]\n");
}

int search_for_patients(int *nrOfPatients, struct patientInfo patient[])
{
    int option, patientNr=-1;
    printf("\n1 Personnummer\n2 Namn\n3 Bildreferens\n");
    printf("Vad vill du s\x94ka med? ");
    scanf("%d", &option);
    while((getchar()) != '\n');
    switch(option)
    {
        case 1: patientNr = search_ssNumber(*&nrOfPatients, patient);
        break;
        case 2: patientNr = search_name(*&nrOfPatients, patient);
        break;
        case 3: patientNr = search_image_reference(*&nrOfPatients, patient);
        break;
        default: printf("V\x84lj en av alternativen\n");
    }
    return patientNr;
}

int search_ssNumber(int *nrOfPatients, struct patientInfo patient[])
{
    int nrOfPatientsShown=0, patientNr;
    char searchStr[SSNUMBER_LENGTH+1];
    printf("Vilket personnummer s\x94ker du efter? ");
    fgets(searchStr, SSNUMBER_LENGTH+1, stdin);
    for(int i=0;i<SSNUMBER_LENGTH+1;i++)
    {
        if(searchStr[i]=='\n')
        {
            searchStr[i] = '\0';
        }
    }
    if(strlen(searchStr)==SSNUMBER_LENGTH)
    {
        while((getchar()) != '\n');
    }
    
    for(int i=0;i<*nrOfPatients;i++)
    {
        if((strcmp(searchStr, patient[i].ssNumber)) == 0)
        {
            printf("\nPersonnummer             Namn                     Bildreferenser\n");
            printf("_______________________________________________________________\n");
            print_patient_info(i, patient);
            patientNr = i;
            nrOfPatientsShown++;
            break;
        }
    }
    if(nrOfPatientsShown==1)
    {
        return patientNr;
    }
    else
    {
        return -1;
    }
}

int search_name(int *nrOfPatients, struct patientInfo patient[])
{
    int nrOfPatientsShown=0, patientNr;
    char searchStr[NAME_LENGTH+1];
    printf("Vilket namn vill du s\x94ka efter? ");
    fgets(searchStr, NAME_LENGTH+1, stdin);
    for(int i=0;i<NAME_LENGTH+1;i++)
    {
        if(searchStr[i]=='\n')
        {
            searchStr[i] = '\0';
        }
    }
    if(strlen(searchStr)==NAME_LENGTH)
    {
        while((getchar()) != '\n');
    }
    
    printf("\nPersonnummer             Namn                     Bildreferenser\n");
    printf("_______________________________________________________________\n");
    for(int i=0;i<*nrOfPatients;i++)
    {
        int strLength;
        char target = searchStr[0];
        char *result = patient[i].name;
        strLength = strlen(searchStr);
        char newStr[20] = {0};

        while((result = strchr(result, target)) != NULL) 
        {
            for(int j=0;j<strLength;j++)
            {
                for(int k=0;k<strLength;k++)
                {
                    newStr[j] = result[j];
                }
            }
            ++result; // Increment result, otherwise we'll find target at the same location
            if((strcmp(newStr, searchStr)) == 0)
            {
                print_patient_info(i, patient);
                patientNr = i;
                nrOfPatientsShown++;
                break;
            }
        }
    }
    if(nrOfPatientsShown==1)
    {
        return patientNr;
    }
    else
    {
        return -1;
    }
}

int search_image_reference(int *nrOfPatients, struct patientInfo patient[])
{
    int searchImage;
    int nrOfPatientsShown=0, patientNr;
    printf("Vilken bildreferens vill du s\x94ka efter? ");
    scanf("%d", &searchImage);
    printf("\nPersonnummer             Namn                     Bildreferenser\n");
    printf("_______________________________________________________________\n");
    for(int i=0;i<*nrOfPatients;i++)
    {
        int j=0;
        while(j<MAX_IMAGES && patient[i].imageReferences[j]!=0)
        {
            if(searchImage == patient[i].imageReferences[j])
            {
                print_patient_info(i, patient);
                patientNr = i;
                nrOfPatientsShown++;
            }
            j++;
        }
    }
    if(nrOfPatientsShown==1)
    {
        return patientNr;
    }
    else
    {
        return -1;
    }
}

void change_patient_data(int *nrOfPatients, struct patientInfo patient[])
{
    int patientNr;
    char end;
    char imageString[MAX_IMAGE_LENGTH+1];
    while(1)
    {
        patientNr = search_for_patients(*&nrOfPatients, patient);
        if(patientNr != -1)
        {
            int i=0, imageNr=1;
            while(imageNr!=0)
            {
                imageNr = patient[patientNr].imageReferences[i];
                i++;
            }
            i=i-1;
            while(i<MAX_IMAGES)
            {
                printf("Ange bildreferens: ");
                fgets(imageString, MAX_IMAGE_LENGTH+1, stdin);
                patient[patientNr].imageReferences[i] = atoi(imageString);
                i++;
                printf("Vill du l\x84gga till en till bildreferens? (y/n): ");
                scanf("%c", &end);
                while((getchar()) != '\n');
                if(end == 'n' || end == 'N')
                {
                    break;
                }
            }
            if(i==MAX_IMAGES)
            {
                printf("Det finns inte plats f\x94r att l\x84gga till fler bildreferenser\n");
            }
            break;
        }
        else
        {
            printf("Vill du s\x94ka igen? (y/n): ");
            scanf("%c", &end);
            while((getchar()) != '\n');
            if(end == 'n' || end == 'N')
            {
                break;
            }
        }
    }
}

void sort_patients(int *nrOfPatients, struct patientInfo patient[])
{
    int option;
    printf("\n1 Sortera efter personnumer\n2 Sortera efter namn\n");
    printf("Hur vill du sortera patientlistan? ");
    scanf("%d", &option);
    while((getchar()) != '\n');
    switch(option)
    {
        case 1: sort_by_number(*&nrOfPatients, patient);
        break;
        case 2: sort_by_name(*&nrOfPatients, patient);
        break;
        default: printf("V\x84lj en av alternativen\n");
    }
}

void sort_by_number(int *nrOfPatients, struct patientInfo patient[])
{
    for(int i=0;i<*nrOfPatients;i++)
    {
        for(int j=0;j<*nrOfPatients-1-i;j++)
        {
            if(strcmp(patient[j].ssNumber, patient[j+1].ssNumber) > 0)
            {
                patient[MAX_PATIENTS+1] = patient[j];
                patient[j] = patient[j+1];
                patient[j+1] = patient[MAX_PATIENTS+1];
            }
        }
    }
}

void sort_by_name(int *nrOfPatients, struct patientInfo patient[])
{
    int nameLength1, nameLength2;
    for(int i=0;i<*nrOfPatients;i++)
    {
        for(int j=0;j<*nrOfPatients-1-i;j++)
        {
            nameLength1 = strlen(patient[j].name);
            nameLength2 = strlen(patient[j+1].name);
            
            if(nameLength1<nameLength2)
            {
                if(strncmp(patient[j].name, patient[j+1].name, nameLength1) > 0)
                {
                    patient[MAX_PATIENTS+1] = patient[j];
                    patient[j] = patient[j+1];
                    patient[j+1] = patient[MAX_PATIENTS+1];
                }
            }
            else
            {
                if(strncmp(patient[j].name, patient[j+1].name, nameLength2) > 0)
                {
                    patient[MAX_PATIENTS+1] = patient[j];
                    patient[j] = patient[j+1];
                    patient[j+1] = patient[MAX_PATIENTS+1];
                }
            }
        }
    }
}

void deregister_patient(int *nrOfPatients, struct patientInfo patient[])
{
    int patientNr;
    char end;
    char imageString[MAX_IMAGE_LENGTH+1];
    while(1)
    {
        patientNr = search_for_patients(*&nrOfPatients, patient);
        if(patientNr != -1)
        {
            printf("\nVill du avregistrera denna patient? (y/n): ");
            scanf("%c", &end);
            while((getchar()) != '\n');
            if(end == 'n' || end == 'N')
            {
                break;
            }
            patient[patientNr] = patient[*nrOfPatients-1];
            (*nrOfPatients)--;
            break;
        }
        else
        {
            printf("Vill du s\x94ka igen? (y/n): ");
            scanf("%c", &end);
            while((getchar()) != '\n');
            if(end == 'n' || end == 'N')
            {
                break;
            }
        }
    }
}

void quit(bool *flag)
{
    *flag = 0;
}

void end_program(char fileGiven[], int *nrOfPatients, struct patientInfo patient[])
{
    int j;
    FILE *fp;
    fp = fopen(fileGiven, "w");
    if(fp!=NULL)
    {
        fprintf(fp, "%d\n", *nrOfPatients);
        for(int i=0;i<*nrOfPatients;i++)
        {
            j=0;
            fprintf(fp, "%s\n", patient[i].ssNumber);
            fprintf(fp, "%s\n", patient[i].name);
            while(j<MAX_IMAGES && patient[i].imageReferences[j] != 0)
            {
                if(patient[i].imageReferences[j+1] == 0 || j == MAX_IMAGES-1)
                {
                    fprintf(fp, "%d\n", patient[i].imageReferences[j]);
                    fprintf(fp, "%d\n", 0);
                    break;
                }
                else
                {
                    fprintf(fp, "%d\n", patient[i].imageReferences[j]);
                    j++;
                }
            }
        }
        fclose(fp);
    }
    else
    {
        printf("Kunde inte \x94ppna filen");
    }
}


/*
Borde gjort så att när man avregistrerar en patient
flyttas alla under upp en nivå så sorteringen av listan
fortsätter vara den samma
*/


//å=\x86   ä=\x84   ö=\x94
//Å=\x8F   Ä=\x8E   Ö=\x99