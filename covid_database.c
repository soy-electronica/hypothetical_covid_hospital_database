
//**  The implementation for the following code was a final project for an electrical & computer engineering programming class at the Univeristy of Maryland College Park. The project due date was three weeks from the date of assignment. The project involves a hypothetical hospital and patient database. The design requirements include managing patients' information. The project helped students master basic build, search, insert, and delete linked list operations in addition to basic graph algorithms, pointer concepts, and dynamic memory allocation **/

//** The following code takes in an input file with either one of the following patient's information: (1) the date a patient checked into the hospital, (2) the date a patient checked out of the hospital, or (3) the deceased date of a patient. The date format in the input file is MM/DD/YY.  The input file also contains a given patient ID (PID) number in addition to another patient’s ID number associated. The additional PID is the who infected that given patient. If the patient does not know who infected them, the additional patient ID will be set as 0. The output file of the code is the user's input query to the database. The information in the input file for each patient is: <date> <PID1> <status> <PID2>. The "date" is the date in which the patient was given some given "status": checked in, checked out, or deceased. **//

//** The program allows the user to input queries: (1) list PIDs of all the patients, (2) search for a patient by PID, (3) find all patients infected by a patient, (4) find the origin of infection of a patient, and (5) find the connection between two patients

//** The program produces an output file with the total number of patients in the database regardless of check in date. The file also provides the current number of patients checked in, where current is relative to the last activity in the input file. The output file also provides the number of deceased, the number of patients who have recovered and checked out, and the average day a recovered patient was checked in. The output file provides the longest COVID infection chain and the date in which the hospital had the maximum amount of patient checked in.


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


//The following are constant variables used as flags throughout the code
#define DECEASED 0
#define CHECKED_OUT 1
#define CHECKED_IN 2
#define ZERO_PATIENT 99999
#define MAX_LINE 800
#define START -9999

#define EXIT 0
#define QUERY_1 1
#define QUERY_2 2
#define QUERY_3 3
#define QUERY_4 4
#define QUERY_5 5
#define DOES_NOT -999999
#define DOES_TOO -5556667
#define HAS_INFECTED_PEOPLE -999999
#define HAS_NOT_INFECTED_PEOPLE -888888

//The following function allocates memory
#define MALLOC_NEW(x) (x*)malloc(sizeof(x))
#define CHECK_MALLOC(x) if (!(x)) {fprintf(stderr, "ERROR: MALLOC DID NOT WORK\n"); exit(1);}


int num_patient_max;/** num patient max variable will keep track of the maximum number of patients*/
int old_count; /**old count variable will keep track of the previous number of patients*/
int m; /**m variable will keep track of the month with max number of patients checked*/
int d; /**d variable will keep track of the date with max number of patients checked*/
int y; /**y variable willkeep track of the year with max number of patients checked*/

typedef struct patient{
    int PID; /**the patient ID number*/
    int check_in_month;/**month the patient first checked into the hospital */
    int check_in_date; /**day the patient first checked into the hospital  */
    int check_in_year; /**year the patient first checked into the hospital  */
    
    /** The following month, day, and year variables are to keep track of the current status of the patient.
     The status associated with the variables include the date the patient was: checked-in, checked out, or deceased */
    
    int month;/** the month the patient checked in, out, or was decleared deceased**/
    int day; /**the day the parient  checked in, out, or was deceased**/
    int year;  /**Month the patient was checked in, out, or deceased*/
    
    /** The following status and origin variables is used to track the current status of the patieint (1) checked in, (2) out, or (3) deceased and who the patient was infected by. */
    
    int status; /**Patient current status*/
    int origin; /**Patient current status*/
    struct patient *from; /**The pointer that points to the patient who patient was infected from*/
} patient_Info;

typedef struct NODE{
    patient_Info patient; /**Will assign to patient type data*/
    struct NODE *prev;
    struct NODE *next;
} NODE;

typedef struct ROOT{
    int num; /**Number of nodes attached to root*/
    struct NODE *head;
    struct NODE *tail;
} ROOT;

typedef struct GRAPH{
    int num; /**the number of vertices in the graph including the zero patient**/
    ROOT *vertice; /**Each vertice is a root which links  it's own list*/
} GRAPH;


FILE *create_output_file(const char* argv);
GRAPH* make_a_graph(void);
void make_zero_node(GRAPH *all_Patients);
void read_applicants_and_make_list(FILE *applicants_File,
                                   GRAPH *all_Patients);
void separate_line_assign_fields(char *string, GRAPH *all_Patients);
void add_to_memory_to_graph(GRAPH *all_Patients, int vertices);
void print_vertice_list(ROOT *vertice);
void assign_a_node_to_vertice(GRAPH *all_Patients, int num_Vertice,
                              NODE *da_Node);
void add_a_node_to_list(GRAPH *all_Patients, NODE *da_Node, int origin_PID);
void change_status(GRAPH *all_Patients, int status ,int pid_To_Change,
                   int month, int date, int year);
void print_graph(GRAPH *all_Patients);
void do_querys(FILE* output_File, GRAPH *all_Patients);
void write_menu_choices_to_output(FILE* output_File);
int ask_for_query_do_query(FILE *output_File, GRAPH *all_Patients, int which_Round);
void first_query_print_pids(GRAPH *all_Patients, FILE* output_File);
void second_query_find_patient(GRAPH *all_Patients, FILE* output_File,
                               int patient_To_Find);
void third_query_patient_infected(GRAPH *all_Patients, FILE* output_File,
                                  int patient_To_Find);
void fourth_query_find_origin(GRAPH *all_Patients, FILE* output_File,
                                  int patient_To_Find);
void fifth_query_find_connection(GRAPH *all_Patients, FILE* output_File,
                                  int patient_1, int patient_2);
void print_report_to_output(GRAPH *all_Patients, FILE* output_File);
void find_longest_chain(GRAPH *all_Patients, FILE* output_File);
void print_day_with_max_patients(GRAPH *all_Patients, FILE* output_File);
float count_days(int month_1, int month_2, int day_1, int day_2,
                 int year_1, int year_2);
int num_digits_in_int(int num_To_Count);

int main(int argc, const char * argv[]) {
    
    //Main program
 
    int start_patient = 1;
   
    if(argc != 3){
        printf("\tERROR!: Not enough command line arguments. \n");
        printf("\t!!!!!!: Insert input file and output file in the command line. \n");
        exit(0);
    }
    GRAPH *patients_Graph;
    FILE *patients_file;
    FILE *output_File; //The output file
    patients_Graph = make_a_graph();
    
    patients_file = fopen(argv[1], "r");
    if(patients_file == NULL){
       printf("\t\t!!!File Cannot OPEN!!\n");
        exit(0);
    }
    output_File = create_output_file(argv[2]);
    read_applicants_and_make_list(patients_file, patients_Graph);
    do_querys(output_File, patients_Graph);
    printf("\n");
   
    fclose(patients_file);
    fclose(output_File);
    
    /**Free Graph Structure*/
    if(patients_Graph != NULL){
        free(patients_Graph);
    }
    /**Free vertices*/
    return 0;
}

FILE* create_output_file(const char* argv){
    /**Opens a file to write and returns file pointer to the file*/
    FILE *output = NULL;
    output = fopen(argv, "w");
    if(output == NULL){
        printf("ERROR: Cannot open input file\n");
        exit(0);
    }
    return output;
}

void write_menu_choices_to_output(FILE* output_File){
    fprintf(output_File, "\n\nWelcome to CoVID-19 Patient Database!\n\n");
    fprintf(output_File, "1:\tList PIDs of all the patients\n");
    fprintf(output_File, "2:\tSearch for a patient by PID\n");
    fprintf(output_File, "3:\tFind all patients infected by a patient\n");
    fprintf(output_File, "4:\tFind the origin of infection of a patient\n");
    fprintf(output_File, "5:\tFind the connection between two patients\n");
    fprintf(output_File, "0:\tEXIT\n\n");
}
void do_querys(FILE* output_File, GRAPH *all_Patients){
    int input_Query = START;
    write_menu_choices_to_output(output_File);
    printf("\n\nWelcome to CoVID-19 Patient Database!\n\n");
    printf("1:\tList PIDs of all the patients\n");
    printf("2:\tSearch for a patient by PID\n");
    printf("3:\tFind all patients infected by a patient\n");
    printf("4:\tFind the origin of infection of a patient\n");
    printf("5:\tFind the connection between two patients\n");
    printf("0:\tEXIT\n\n");
    int which_Input_Number = 0;/**Keeps track of the number of inputs the user puts*/
  
    ///**This loop continues to ask for a query and peforms the query*/
    while(input_Query != 0){
        which_Input_Number++;
        input_Query = ask_for_query_do_query(output_File, all_Patients, which_Input_Number);
    }
    print_report_to_output(all_Patients, output_File);
    
}

void print_day_with_max_patients(GRAPH *all_Patients, FILE* output_File){
    /**This function prints the date with the max number of patients checked in*/
  
    fprintf(output_File ,"On %.2d/%.2d/%.4d, the hospital has a maximal of %d patients\n",
           m, d, y, num_patient_max);
    
}
void find_longest_chain(GRAPH *all_Patients, FILE* output_File){
    /**This function finds the longest chain of infection by tranversing the "from" patients
     of each vertices*/
    int chain_Num_Old = 0, chain_Num_New = 0;
    int i;
    patient_Info *curr, *temp;
    patient_Info *source_1, *source_2;
    NODE *longest_Chain;
    
    fprintf(output_File ,"One longest CoVID infection chain:\n");
    for(i = 0; i < all_Patients->num; i++){
        /**Loops through all the the vertices in the graph. The source, or from, of the vertices
         serves as a linked list. The pointer curr starts at the source, or from field pointer,
         of each vertice[ i ] and traverses to each "from" and keeps count of how many times it tranverses.
         The number of time it tranverses is the number of nodes in that chain. The last longest chain is printed**/
        source_1 = all_Patients->vertice[i].head->patient.from;
        for(curr = source_1; curr != NULL; curr = curr->from){
            chain_Num_New++;
        }
        if(chain_Num_New > chain_Num_Old){
            chain_Num_Old = chain_Num_New; /**Makes the new chain count the old chain
                                            count if the new count is larger.*/
            longest_Chain = all_Patients->vertice[i].head; /**Assigns the longest chain*/
        }
        chain_Num_New = 0; /**New chain count resets to zero to check the next iteration*/
    }
    
    /**The longest chain is kept. It is then printed out*/
    source_2 = &longest_Chain->patient;
    fprintf(output_File,"%.6d", source_2->PID);

    for(curr = source_2->from; curr != NULL; curr = curr->from){
        fprintf(output_File," <- %.6d", curr->PID);
    }
    fprintf(output_File,"\n\n");
}

void print_report_to_output(GRAPH *all_Patients, FILE* output_File){
    
    int num_Deceased = 0;
    int num_In_Hospital= 0;
    float num_Checked_Out = 0;
    int i, line_Count = 0;
    float sum_Of_days; /**The sum of each day a patient was checked in*/
    int month1, month2, day1, day2, year1, year2;
    float difference; /**The diference in days of check in and check out date*/
    float average_Days;
    
    fprintf(output_File, "PART TWO INFORMATION:\n\n");
    fprintf(output_File, "This CoVID-19 Patient Database has %d patients.\n\n",
            (all_Patients->num)-1);
    
    /**Prints checked in patients*/
    for(i = 1; i < all_Patients->num; i++) {
        /**Find the number of checked in patients*/
        if(all_Patients->vertice[i].head->patient.status == CHECKED_IN){
            num_In_Hospital++;
        }
    }
    fprintf(output_File ,"%d patients currently in the hospital:\n", num_In_Hospital);
    for(i = 1; i < all_Patients->num; i++) {
        /**Prints the checked inpatients*/
        if(all_Patients->vertice[i].head->patient.status == CHECKED_IN){
            line_Count++;
            fprintf(output_File ,"%.6d ", all_Patients->vertice[i].head->patient.PID);
            if( (line_Count % 5) == 0){
                /**Prints a new line every fifth deceased patient*/
                fprintf(output_File, "\n");
            }
        }
    }
    fprintf(output_File, "\n\n");
    
    line_Count = 0;
    /**Prints deceased patients*/
    for(i = 1; i < all_Patients->num; i++) {
        /**Find the number of deceased patients*/
        if(all_Patients->vertice[i].head->patient.status == DECEASED){
            num_Deceased++;
        }
    }
    fprintf(output_File ,"%d patients deceased:\n", num_Deceased);
    for(i = 1; i < all_Patients->num; i++) {
        /**Prints the deceased patients*/
        if(all_Patients->vertice[i].head->patient.status == DECEASED){
            line_Count++;
            fprintf(output_File ,"%.6d ", all_Patients->vertice[i].head->patient.PID);
            if( (line_Count % 5) == 0){
                /**Prints a new line every fifth deceased patient*/
                fprintf(output_File, "\n");
            }
        }
    }
    fprintf(output_File, "\n\n");
    
    line_Count = 0;
    /**Prints recovered patients*/
    for(i = 1; i < all_Patients->num; i++) {
        /**Find the number of patients checked out**/
        if(all_Patients->vertice[i].head->patient.status == CHECKED_OUT){
            num_Checked_Out++;
        }
    }
    fprintf(output_File ,"%d patients have recovered and checked out:\n", (int) num_Checked_Out);
    for(i = 1; i < all_Patients->num; i++) {
        /**Prints the checked out patients*/
        if(all_Patients->vertice[i].head->patient.status == CHECKED_OUT){
            line_Count++; /**Updates the line count when a check out patient is printed*/
            month1 = all_Patients->vertice[i].head->patient.check_in_month;
            month2 = all_Patients->vertice[i].head->patient.month;
            day1 = all_Patients->vertice[i].head->patient.check_in_date;
            day2 = all_Patients->vertice[i].head->patient.day;
            year1 = all_Patients->vertice[i].head->patient.check_in_year;
            year2 = all_Patients->vertice[i].head->patient.year;
            fprintf(output_File ,"%.6d ", all_Patients->vertice[i].head->patient.PID);
            difference = count_days(month1, month2, day1, day2, year1, year2);
            sum_Of_days += difference;
            if((line_Count % 5) == 0){
                /**Prints a new line every fifth deceased patient*/
                fprintf(output_File, "\n");
            }
        }
    }
    fprintf(output_File, "\n");
    average_Days = sum_Of_days/num_Checked_Out;
    fprintf(output_File, "Average days in the hospital: %.2f \n\n", average_Days);
    
    find_longest_chain(all_Patients, output_File);
    print_day_with_max_patients(all_Patients, output_File);

}

void fifth_query_find_connection(GRAPH *all_Patients, FILE* output_File,
                                 int patient_1, int patient_2){
    /**This function finds the connection between two patients, patient 1 and patient 2. If there is a
     a connection, the connection is printed out*/
    int is_There_A_Connection = DOES_NOT;
    int i, j, k, first_Flag = DOES_NOT;
    int vert1_Patient_ID, vert2_Patient_ID;
    int num_Digits_Of_Patient_1 = -1;
    int num_Digits_Of_Patient_2 = -1;
    int does_Patient_Exist = DOES_TOO;
    int does_Patient_Infect = HAS_NOT_INFECTED_PEOPLE;
    int is_Pat1_In_Database = DOES_NOT, is_Pat2_In_Database = DOES_NOT;
    int connected_PID;
    patient_Info *curr, *temp;
    patient_Info *source_1, *source_2;
    
    printf("\nFind the connection between patients %.6d and %.6d\n", patient_1, patient_2);
    fprintf(output_File,"\nFind the connection between patients %.6d and %.6d\n", patient_1, patient_2);
    
    num_Digits_Of_Patient_1 = num_digits_in_int(patient_1);
    num_Digits_Of_Patient_2 = num_digits_in_int(patient_2);
    if(num_Digits_Of_Patient_1 > 6 || patient_1< 0){
        /**Checks if the patient 1 entered by the user is valid or not*/
        does_Patient_Exist = DOES_NOT;
        printf("Patient %.6d not found.\n\n", patient_1);
        fprintf(output_File, "\t\tPatient %.6d not found.\n\n", patient_1);
        return;
        
    }
    if(num_Digits_Of_Patient_2 > 6 || patient_2< 0){
        /**Checks if the patient 2 entered by the user is valid or not*/
        does_Patient_Exist = DOES_NOT;
        printf("Patient %.6d not found.\n\n", patient_2);
        fprintf(output_File, "Patient %.6d not found.\n\n", patient_2);
        return;
    }
    
    for(i = 0; i < all_Patients->num; i++){ /**Checks if patients are in the graogh*/
        vert1_Patient_ID = all_Patients->vertice[i].head->patient.PID;
        if(vert1_Patient_ID == patient_1){
            is_Pat1_In_Database = DOES_TOO;
            /**The Patient is in the database */
        }
        if(vert1_Patient_ID == patient_2){
            is_Pat2_In_Database = DOES_TOO;
            /**The Patient is in the database */
        }
    }
    
    /**Print the patient that is not in database*/
    if(is_Pat1_In_Database == DOES_NOT){
        does_Patient_Exist = DOES_NOT;
        printf("%.6d is not a patient\n\n", patient_1);
        fprintf(output_File, "%.6d is not a patient\n\n", patient_1);
        return;
    }
    if(is_Pat2_In_Database == DOES_NOT){
        does_Patient_Exist = DOES_NOT;
        printf("%.6d is not a patient\n\n", patient_2);
        fprintf(output_File, "%.6d is not a patient\n\n", patient_2);
        return;
    }
    
    if(is_Pat1_In_Database == DOES_TOO && is_Pat2_In_Database == DOES_TOO){
        /**Connection if....*/
        /**The two patients exist. A connection will tried to be found*/
        does_Patient_Exist = DOES_TOO;
        for(i = 0; i < all_Patients->num; i++){
            vert1_Patient_ID = all_Patients->vertice[i].head->patient.PID;
            if(vert1_Patient_ID == patient_1){
                /**Saves the source of patient 1*/
                source_1 = all_Patients->vertice[i].head->patient.from;
            }
            if(vert1_Patient_ID == patient_2){
                /**Saves the source of patient 2*/
                source_2 = all_Patients->vertice[i].head->patient.from;
            }
        } /**End of looping through graph to get first source*/
        
        if(source_1->PID == patient_2){
            is_There_A_Connection = DOES_TOO;
            connected_PID = patient_2; /**The common source of infection is the patient 2*/
            printf("%.6d ", patient_1);
            fprintf(output_File ,"%.6d ", patient_1);
            for(curr = source_1; curr != NULL; curr = curr->from){
                printf("<- %.6d ", curr->PID);
                fprintf(output_File, "<- %.6d ", curr->PID);
                if(curr->PID == connected_PID){
                    break; /**Breaks loop up after the common connector PID is found*/
                }
            }
            printf("\n");
            fprintf(output_File, "\n");
            printf("%.6d \n", patient_2);
            fprintf(output_File,"%.6d \n", patient_2);
            return;
        } else {
            if(source_2->PID == patient_1){
                is_There_A_Connection = DOES_TOO;
                connected_PID = patient_1; /**The common source of infection is the patient 1*/
                printf("%.6d ", patient_2);
                fprintf(output_File ,"%.6d ", patient_2);
                for(curr = source_2; curr != NULL; curr = curr->from){
                    printf("<- %.6d ", curr->PID);
                    fprintf(output_File, "<- %.6d ", curr->PID);
                    if(curr->PID == connected_PID){
                        break; /**Breaks loop up after the common connector PID is found*/
                    }
                }
                printf("\n");
                fprintf(output_File, "\n");
                printf("%.6d \n", patient_1);
                fprintf(output_File,"%.6d \n", patient_1);
                return;
                
            } else {
                /**Does a double tranverse through the sources of each patient to see if there is a connection. Once
                 the first connection is found, the loops break. The zero patient is ignored as a connection*/
                for(curr = source_1; curr != NULL; curr = curr->from){
                    if(first_Flag == DOES_TOO){
                        /**Loop breaks when one connection is already found*/
                        break;
                    }
                    for(temp = source_2; temp !=NULL; temp = temp->from){
                        if(temp->PID == 0 || curr->PID == 0){
                            break; /**Skips zero node as a connection*/
                        }
                        if(curr->PID == temp->PID){
                            /**If there is a match between the souces, then there is a connection*/
                            is_There_A_Connection = DOES_TOO;
                            connected_PID = curr->PID;
                            /**Sets the flag to break out the outer loop*/
                            first_Flag = DOES_TOO;
                            break;
                        }
                    }
                }
            }
        }

        if(is_There_A_Connection == DOES_TOO){
            /**If there was a connection found, print up till the first connection with pid connected_PID
             for both patients*/
            printf("%.6d ", patient_1);
            fprintf(output_File ,"%.6d ", patient_1);
            for(curr = source_1; curr != NULL; curr = curr->from){
                printf("<- %.6d ", curr->PID);
                fprintf(output_File, "<- %.6d ", curr->PID);
                if(curr->PID == connected_PID){
                    break; /**Breaks loop up after the common connector PID is found*/
                }
            }
            printf("\n");
            fprintf(output_File, "\n");
            
            printf("%.6d ", patient_2);
            fprintf(output_File, "%.6d ", patient_2);
            for(temp = source_2; temp !=NULL; temp = temp->from){
                printf("<- %.6d ", temp->PID);
                fprintf(output_File, "<- %.6d ", temp->PID);
                if(temp->PID == connected_PID){
                    break; /**Breaks loop up after the common connector PID is found*/
                }
            }
            printf("\n\n");
            fprintf(output_File, "\n\n");
        } /**End of print up until common connection if*/
        
    } /**End of finding connection if*/

    if(does_Patient_Exist == DOES_TOO && is_There_A_Connection == DOES_NOT){
        /**Prints if the two existing patients in database does not have a connection*/
        printf("%.6d and %.6d do not have any connection.\n\n", patient_1 ,patient_2);
        fprintf(output_File, "%.6d and %.6d do not have any connection.\n\n", patient_1 ,patient_2);
    }
    
    
}

void fourth_query_find_origin(GRAPH *all_Patients, FILE* output_File,
                              int patient_To_Find){
    int i, j, k;
    int vertice_Patient_ID, num_Digits_Of_Patient_To_Find = -966667779;
    int does_Patient_Exist = DOES_NOT;
    int does_Patient_Infect = HAS_NOT_INFECTED_PEOPLE;
    patient_Info *curr;
    patient_Info *source_Stucture;
    
    /**Checks if the patient entered by the user is valid patient ID or not*/
    num_Digits_Of_Patient_To_Find = num_digits_in_int(patient_To_Find);
    if(num_Digits_Of_Patient_To_Find > 6 || patient_To_Find < 0){
        does_Patient_Exist = DOES_NOT;
    } else {
        printf("Find the origin of infection of a patient %.6d\n", patient_To_Find);
        fprintf(output_File,"Find the origin of infection of a patient %.6d\n", patient_To_Find);
        for(i = 0; i < all_Patients->num; i++){
            /**Loops through all the the vertices in the graph. Once the loop comes across the
             patient whose origin of infection needs to found, we tranverse using the "from" of this patient
             till NULL is reached. Each from that is encounted as the "froms" are tranversed are printed out.**/
            vertice_Patient_ID = all_Patients->vertice[i].head->patient.PID;
            if(vertice_Patient_ID == patient_To_Find){
                does_Patient_Exist = DOES_TOO;
                /**The Patient is in the database */
            }
            source_Stucture = all_Patients->vertice[i].head->patient.from;
            if(vertice_Patient_ID == patient_To_Find){
                does_Patient_Infect = HAS_INFECTED_PEOPLE; /**This patient has infected this vertice*/
                /**Once the vertice is located, the list of from is tranversed*/
                printf("%.6d ", vertice_Patient_ID);
                fprintf(output_File,"%.6d ", vertice_Patient_ID);
                for(curr = source_Stucture; curr != NULL; curr = curr->from){
                   printf("<- %.6d ", curr->PID);
                   fprintf(output_File, "<- %.6d ", curr->PID);
                }
            }
        }
        if(does_Patient_Infect == HAS_INFECTED_PEOPLE) {
            printf("\n\n");
            fprintf(output_File, "\n\n");
        }
       
    }
    
    if(does_Patient_Exist == DOES_NOT){
        printf("Patient %.6d not found.\n\n", patient_To_Find);
        fprintf(output_File, "Patient %.6d not found.\n\n", patient_To_Find);
    }
  
}

void third_query_patient_infected(GRAPH *all_Patients, FILE* output_File,
                                  int patient_To_Find){
    /**This function finds the patients that have been directly and indriectly infected by patient with ID
     passed in as patient_To_Find.*/
    int does_Patient_Exist = DOES_NOT;
    int does_Patient_Infect = HAS_NOT_INFECTED_PEOPLE;
    int i, j = 0, k, num_Digits_Of_Patient_To_Find;
    int vertice_Patient_ID;
    patient_Info *curr;
    patient_Info *source_Stucture;
    num_Digits_Of_Patient_To_Find = num_digits_in_int(patient_To_Find);
    
    if(num_Digits_Of_Patient_To_Find > 6 || patient_To_Find < 0){
        /**Checks if the patient entered by the user is valid or not*/
        does_Patient_Exist = DOES_NOT;
    } else {
        printf("Find all patients infected by a patient %.6d\n", patient_To_Find);
        fprintf(output_File,"Find all patients infected by a patient %.6d\n", patient_To_Find);
        for(i = 0; i < all_Patients->num; i++){
            /**Loops through all the the vertices in the graph. The source, or from, of the vertices
             serves as a linked list. The pointer curr starts at the source, or from field pointer,
             of each vertice[ i ] and traverses to each "from" to check whether if "from" is equal
             to the patient_To_Find. If a from is equal to the patient_To_Find, then this vertices, vertice[ i ],
             has been infected by patient_To_Find. The vertice [ i ] is then printed.**/
            vertice_Patient_ID = all_Patients->vertice[i].head->patient.PID;
            source_Stucture = all_Patients->vertice[i].head->patient.from;
            
            if(vertice_Patient_ID == patient_To_Find){
                does_Patient_Exist = DOES_TOO;
                /**The Patient is in the database */
            }
            
            for(curr = source_Stucture; curr != NULL; curr = curr->from){
                if(curr->PID == patient_To_Find){
                    does_Patient_Infect = HAS_INFECTED_PEOPLE;
                    printf("%.6d ", vertice_Patient_ID);
                    fprintf(output_File, "%.6d ", vertice_Patient_ID);
                    j++;
                    if( (j % 5) == 0){
                        /**Prints a new line every fifth j iteration*/
                        printf("\n");
                        fprintf(output_File, "\n");
                    }
                    
                }
            }
        
        }
        
        if(does_Patient_Infect == HAS_INFECTED_PEOPLE){
            printf("\n\n");
            fprintf(output_File, "\n\n");
        }
    }
    if(does_Patient_Exist == DOES_NOT){
        printf("Patient %.6d not found.\n\n", patient_To_Find);
        fprintf(output_File, "Patient %.6d not found.\n\n", patient_To_Find);
    } else {
        if(does_Patient_Infect == HAS_NOT_INFECTED_PEOPLE){
            printf("Patient %.6d has not infected anyone.\n\n", patient_To_Find);
            fprintf(output_File, "Patient %.6d has not infected anyone.\n\n", patient_To_Find);
        }
    }
    
}

void second_query_find_patient(GRAPH *all_Patients, FILE* output_File,
                               int patient_To_Find){
    /**This function finds a patient ID in the graph and prints out the patient information*/
    int i, j, k;
    int max_Date_Month_Digits = 2, num_Digits_In_Field;
    int max_Digits_ID = 6;
    int num_Digits_Of_Patient_To_Find;
    int does_Patient_Exist = -999999;
    
    num_Digits_Of_Patient_To_Find = num_digits_in_int(patient_To_Find);
  
    printf("Search for patient with PID %.6d\n", patient_To_Find);
    fprintf(output_File, "Search for patient with PID %.6d\n", patient_To_Find);
   
    if(num_Digits_Of_Patient_To_Find > 6 || patient_To_Find < 0){
        /**Checks if the patient entered by the user is valid or not*/
        does_Patient_Exist = DOES_NOT;
    } else {
        for(i = 1; i < all_Patients->num; i++) {
            if(all_Patients->vertice[i].head->patient.PID == patient_To_Find){
                does_Patient_Exist = DOES_TOO;
                switch(all_Patients->vertice[i].head->patient.status){
                    case CHECKED_IN:
                        printf("%.6d: checked in: ", all_Patients->vertice[i].head->patient.PID);
                        fprintf(output_File, "%.6d: checked in: ", all_Patients->vertice[i].head->patient.PID);
                        printf("%.2d/", all_Patients->vertice[i].head->patient.check_in_month);
                        fprintf(output_File, "%.2d/", all_Patients->vertice[i].head->patient.check_in_month);
                        /**Prints out check in date accordingly */
                        printf("%.2d/", all_Patients->vertice[i].head->patient.check_in_date);
                        fprintf(output_File, "%.2d/", all_Patients->vertice[i].head->patient.check_in_date);
                        /**Prints out check in year accordingly */
                        printf("%d; ", all_Patients->vertice[i].head->patient.check_in_year);
                        fprintf(output_File, "%d; ", all_Patients->vertice[i].head->patient.check_in_year);
                        /**Prints out status accordingly*/
                        printf("in hospital.\n\n");
                        fprintf(output_File, "in hospital.\n\n");
                        break;
                    case CHECKED_OUT:
                        /**Prints out Patient id accordingly */
                        printf("%.6d: checked in: ", all_Patients->vertice[i].head->patient.PID);
                        fprintf(output_File, "%.6d: checked in: ", all_Patients->vertice[i].head->patient.PID);
                        /**Prints out check in month accordingly */
                        printf("%.2d/", all_Patients->vertice[i].head->patient.check_in_month);
                        fprintf(output_File, "%.2d/", all_Patients->vertice[i].head->patient.check_in_month);
                        /**Prints out check in date accordingly */
                        printf("%.2d/", all_Patients->vertice[i].head->patient.check_in_date);
                        fprintf(output_File, "%.2d/", all_Patients->vertice[i].head->patient.check_in_date);
                        /**Prints out check in year accordingly */
                        printf("%d; ", all_Patients->vertice[i].head->patient.check_in_year);
                        fprintf(output_File, "%d; ", all_Patients->vertice[i].head->patient.check_in_year);
                        /**Prints out check out status accordingly*/
                        printf("checked out: ");
                        fprintf(output_File, "checked out: ");
                        /**Prints out check out month accordingly */
                        printf("%.2d/", all_Patients->vertice[i].head->patient.month);
                        fprintf(output_File, "%.2d/",
                                all_Patients->vertice[i].head->patient.month);
                        /**Prints out check out date accordingly */
                        printf("%d/", all_Patients->vertice[i].head->patient.day);
                        fprintf(output_File, "%d/", all_Patients->vertice[i].head->patient.day);
                        /**Prints out check out year accordingly */
                        printf("%d\n\n", all_Patients->vertice[i].head->patient.year);
                        fprintf(output_File, "%d\n\n", all_Patients->vertice[i].head->patient.year);
                        break;
                    case DECEASED:
                        /**Prints out Patient check in info accordingly */
                        printf("%.6d: checked in: ", all_Patients->vertice[i].head->patient.PID);
                        fprintf(output_File, "%.6d: checked in: ", all_Patients->vertice[i].head->patient.PID);
                
                        /**Prints out check in month accordingly */
                        printf("%.2d/", all_Patients->vertice[i].head->patient.check_in_month);
                        fprintf(output_File, "%.2d/", all_Patients->vertice[i].head->patient.check_in_month);
                        /**Prints out check in date accordingly */
                        printf("%.2d/", all_Patients->vertice[i].head->patient.check_in_date);
                        fprintf(output_File, "%.2d/", all_Patients->vertice[i].head->patient.check_in_date);
                        /**Prints out check in year accordingly */
                        printf("%d; ", all_Patients->vertice[i].head->patient.check_in_year);
                        fprintf(output_File, "%d; ", all_Patients->vertice[i].head->patient.check_in_year);
                        /**Prints out the deceased date accordingly*/
                        printf("deceased: ");
                        fprintf(output_File, "deceased: ");
                        /**Prints out deceased month accordingly */
                        printf("%.2d/", all_Patients->vertice[i].head->patient.month);
                        fprintf(output_File, "%.2d/",
                                all_Patients->vertice[i].head->patient.month);
                        /**Prints out deceased day accordingly */
                        printf("%.2d/", all_Patients->vertice[i].head->patient.day);
                        fprintf(output_File, "%.2d/", all_Patients->vertice[i].head->patient.day);
                        /**Prints out deceased year accordingly */
                        printf("%d.\n\n", all_Patients->vertice[i].head->patient.year);
                        fprintf(output_File, "%d.\n\n", all_Patients->vertice[i].head->patient.year);
                        break;
                }
            }
        }
    }
    if(does_Patient_Exist == DOES_NOT){
        printf("Patient %.6d not found.\n\n", patient_To_Find);
        fprintf(output_File, "Patient %.6d not found.\n\n", patient_To_Find);
    }
}

void first_query_print_pids(GRAPH *all_Patients, FILE* output_File){
    /**This function prints out the pid of each patient in the graph onto the screen and the output file*/
    
    int i, j, k;
    int zero = 0;
    int num_Digits_In_PID, max_Digits = 6, print_This_Many_Zeros;
    printf("List PIDs of all the patients\n");
    fprintf(output_File, "List PIDs of all the patients\n");
    for(i = 0; i < all_Patients->num; i++){
        if(i == 0){
            continue;
        }
        printf("%.6d ",all_Patients->vertice[i].head->patient.PID);
        fprintf(output_File,"%.6d ",all_Patients->vertice[i].head->patient.PID);
        if( (i % 5) == 0){
            /**Prints a new line every fifth iteration*/
            printf("\n");
            fprintf(output_File, "\n");
        }
    }
    printf("\n\n");
    fprintf(output_File, "\n\n");
}


int ask_for_query_do_query(FILE *output_File, GRAPH *all_Patients,
                           int which_Round){
    /**This function asks the user to input a query choice and then peforms that query choice. The query choice
     is returned.*/
    int current_Query = START;
    int patient_ID, patient_ID_2;
    
    if(which_Round > 0 ) {
        printf("\nEnter you choice (0-5)\n\n");
        printf("1:\tList PIDs of all the patients\n");
        printf("2:\tSearch for a patient by PID\n");
        printf("3:\tFind all patients infected by a patient\n");
        printf("4:\tFind the origin of infection of a patient\n");
        printf("5:\tFind the connection between two patients\n");
        printf("0:\tEXIT\n\n");
        scanf("%d", &current_Query);
    }

    if(current_Query > 5 || current_Query < 0){
        printf("ERROR: Not A valid choice! Please choose a valid choice!\n");
        fprintf(output_File,"%d: A invalid choice was entered\n\n", which_Round);
        return current_Query;
    } else {
        fprintf(output_File,"%d: ", which_Round);
        switch(current_Query){
            case EXIT:
                printf("Exiting Program!\n");
                fprintf(output_File,"EXIT\n\n");
                fprintf(output_File,"Thank you for using the CoVID-19 Patient Database!\n");
                //exit(0);
                break;
            case QUERY_1:
                first_query_print_pids(all_Patients, output_File);
                break;
            case QUERY_2:
                printf("\nEnter the patient id: ");
                printf("\n");
                scanf("%d", &patient_ID);
                printf("%d: ", which_Round);
                second_query_find_patient(all_Patients, output_File, patient_ID);
                break;
            case QUERY_3:
                printf("\nEnter the patient id: ");
                printf("\n");
                scanf("%d", &patient_ID);
                third_query_patient_infected(all_Patients, output_File, patient_ID);
                break;
            case QUERY_4:
                printf("\nEnter the patient id: ");
                printf("\n");
                scanf("%d", &patient_ID);
                fourth_query_find_origin(all_Patients, output_File, patient_ID);
                break;
            case QUERY_5:
                printf("\nEnter the patient id for the two patients: ");
                scanf("%d %d", &patient_ID, &patient_ID_2);
                fifth_query_find_connection(all_Patients, output_File,
                                            patient_ID, patient_ID_2);
                break;
        }
    }
    return current_Query;
}

void read_applicants_and_make_list(FILE *applicants_File,
                                   GRAPH *all_Patients){
    /**This function takes in the input patient file and the graph list data
    structure. The function extracts each individual character from the file , stores the characters of each line
    into one big string, and passes the big string to the separate_assign_line() function.
    The separate_line_assign_fields() function extracts the fields accordingly from the big string**/
    num_patient_max = 0;
    old_count = 0;
    m = 0;
    d = 0;
    y = 0;
    char charr = 0;
    int string_Size = 0;
    char line_String[MAX_LINE]; /**The max number of line will be finite since the max
                                 patient id is 6 integers. The check in, check out, and deceased
                                 words are also finite. It is safe to assume that a line will not exceed
                                 this number of characters*/
    int line_Index = 0;
    char *line;

    while ((charr = fgetc(applicants_File)) != EOF){
        if(charr == '\n'){
            /** When a new line is encountered, new information needs to be stored into the list data stricture.
            The string and index for the string that contains the line also needs to be reseted in order to
             store in the next new line */
            line_String[line_Index] = '\0';
            line_Index = 0;/**Resests line string index in order to read the next line**/
            separate_line_assign_fields(line_String, all_Patients);
            /**Extracts and assigns fields to applicants structure **/
            strcpy(line_String, "");/**Resets the big string to store the next line**/
        } else {
            line_String[line_Index] = charr;
            line_Index++;
        }
    
    } /** End of read file character by character while loop **/
}

void separate_line_assign_fields(char *string, GRAPH *all_Patients){
    /**This fucntion takes in a string and parses through that string to: check in, check out, or mark
     a patient as deceased. The string being passed in is a line read in from the input file. Each patient will
     have a vertice in the graph.*/
    int i;
    char string_1[40] = "05/04/2020 23617 check out";
    char check_in_out[8];
    char status [10];
    int status_Int;
    char check[10] = "check";
    char check_out[10] = "out";
    char check_in[10] = "in";
    char deceased[10] = "deceased";
    int pid, month, day, year, source;
    int compare_check_in_out;
    int num_Patients; /***Number of patients in graph **/
    patient_Info new_Patient;
    NODE *new;
    new = MALLOC_NEW(NODE);
    CHECK_MALLOC(new);
    if(all_Patients->num == 0){
       // printf("In zero if....\n");
        make_zero_node(all_Patients);
        all_Patients->num = 1; /**Since zero patient has been assigned as a vertice,
                                the number of patients will include the zero patient*/
        num_Patients = 1;
    }
  
    /**parses the string and extracts values accordingly*/

    sscanf(string, "%d/%d/%d %d %[^ ]", &month, &day, &year, &pid, status);
    /**Checks if the patient is checking out*/
    compare_check_in_out = strcmp(status, deceased);
    if(compare_check_in_out == 0){
        change_status(all_Patients, DECEASED, pid, month, day, year);
        num_patient_max--;
    } else {
        sscanf(string, "%d/%d/%d %d %[^ ] %[^ ]", &month, &day, &year, &pid, status, check_in_out);
            compare_check_in_out = strcmp(check_out, check_in_out);
            if(compare_check_in_out == 0){
                change_status(all_Patients, CHECKED_OUT, pid, month, day, year);
                num_patient_max--;
                /**Check out patient */
            } else {
                /**Checks if the patient is checking in*/
                compare_check_in_out = strcmp(check_in, check_in_out);
                if (compare_check_in_out == 0) {
                    status_Int = CHECKED_IN;
                    num_patient_max++;
                    if(num_patient_max > old_count){
                        old_count = num_patient_max;
                        m = month;
                        d = day;
                        y = year;
                    }
                    num_Patients = (all_Patients->num);
                    sscanf(string, "%d/%d/%d %d %[^ ] %[^ ] %d", &month, &day,
                           &year, &pid, status ,check_in_out, &source);
                    new_Patient.PID = pid;
                    new_Patient.month = month;
                    new_Patient.day = day;
                    new_Patient.year = year;
                    new_Patient.check_in_month = month;
                    new_Patient.check_in_date = day;
                    new_Patient.check_in_year = year;
                    new_Patient.status = status_Int;
                    new_Patient.origin = source;
                    new->patient = new_Patient;
                    new->next = NULL;
                    add_to_memory_to_graph(all_Patients, num_Patients+1); /**Adds memory of array of roots*/
                    assign_a_node_to_vertice(all_Patients, num_Patients, new); /** */
                    add_a_node_to_list(all_Patients, new, source);
                    
                }
            }
    }
    strcpy(string, "");
}
    
void change_status(GRAPH *all_Patients, int status ,int pid_To_Change,
                   int month, int date, int year){
    /**This function changes the status and date of the patient that needs to be changed in the vertices.
     The status is also hanged in the linked list where the patient was infected from, if any **/
    int i, j;
    NODE *curr;
    
    
    for(i = 1; i < all_Patients->num; i++){
        if(all_Patients->vertice[i].head->patient.PID == pid_To_Change){
            /**Finds the vertices whose status needs to be changed to check out*/
            if(status == CHECKED_OUT){
                all_Patients->vertice[i].head->patient.status = CHECKED_OUT;
                all_Patients->vertice[i].head->patient.month = month;
                all_Patients->vertice[i].head->patient.day = date;
                all_Patients->vertice[i].head->patient.year = year;
                for(j = 0; j < all_Patients->num; j++){
                    /**Loops through array of roots to find the patient's infection source's root.
                     Once the source is found, the list is tranversed till the patient that is being
                     changed is enountered. The fields in the patient being changed is assigned
                     accordingly. **/
                    if (all_Patients->vertice[j].head->patient.PID ==
                        all_Patients->vertice[i].head->patient.from->PID){
                        /**Found the from patient in the array of roots. Now the list
                         of the from patient will be tranversed to change the status in that list*/
                        for(curr = all_Patients->vertice[j].head; curr != NULL; curr = curr->next){
                            if(curr->patient.PID == pid_To_Change){
                                curr->patient.status = CHECKED_OUT;
                                curr->patient.month = month;
                                curr->patient.day = date;
                                curr->patient.year = year;
                                break;
                            }
                        }
                        
                    }
                }
            } else {
                if(status == DECEASED){
                    /**Finds the vertices whose status needs to be changed to deceased*/
                    all_Patients->vertice[i].head->patient.status = DECEASED;
                    all_Patients->vertice[i].head->patient.month = month;
                    all_Patients->vertice[i].head->patient.day = date;
                    all_Patients->vertice[i].head->patient.year = year;
                    curr = all_Patients->vertice[i].head;
                    for(j = 0; j < all_Patients->num; j++){
                        /**Loops through array of roots to find the patient's infection source's root.
                         Once the source is found, the list is tranversed till the patient that is being
                         changed is enountered. The fields in the patient being changed is assigned
                         accordingly. **/
                        if (all_Patients->vertice[j].head->patient.PID ==
                            all_Patients->vertice[i].head->patient.from->PID){
                            /**Found the from patient in the array of roots. Now the list
                             of the from patient will be tranversed to change the status in that list*/
                            for(curr = all_Patients->vertice[j].head; curr != NULL; curr = curr->next){
                                if(curr->patient.PID == pid_To_Change){
                                    curr->patient.status = DECEASED;
                                    curr->patient.month = month;
                                    curr->patient.day = date;
                                    curr->patient.year = year;
                                    break;
                                }
                            }
                            
                        }
                    }
                }
            }
           
        }
    }
}

void add_a_node_to_list(GRAPH *all_Patients, NODE *da_Node, int origin_PID){
    /**This function is called everytime a node needs to be atatched to a vertice. A node attached to
     the vertice is a patient that has been infected by the vertice. This creates a linked list for each vertice */
    NODE *temp;
    NODE *curr;
    int i;
    
    
    temp = MALLOC_NEW(NODE);
    CHECK_MALLOC(temp);
    temp->patient.PID = da_Node->patient.PID;
    temp->patient.status = da_Node->patient.status;
    temp->patient.month = da_Node->patient.month;
    temp->patient.day = da_Node->patient.day;
    temp->patient.year = da_Node->patient.year;
    temp->patient.origin = da_Node->patient.origin;
    temp->next = NULL;
  
    for(i = 0; i < all_Patients->num; i++){
        if(da_Node->patient.origin == all_Patients->vertice[i].head->patient.PID){
            curr = all_Patients->vertice[i].head;
            for(curr = all_Patients->vertice[i].head; curr->next != NULL; curr = curr->next)
                ;
            curr->next = temp;
            all_Patients->vertice[i].num += 1; /**Increments the counts of node attached to head*/
        }
     
    }
}

void print_vertice_list(ROOT *vertice){
    /**This function prints the list attached to the vertice, i.e. the patients infected by the vertice*/
    NODE *curr;
    for(curr = vertice->head; curr != NULL; curr = curr->next){
        printf("%d->", curr->patient.PID);
    }
    printf("/\n");
}

void assign_a_node_to_vertice(GRAPH *all_Patients, int num_Vertice,
                              NODE *da_Node){
    /**This function assigns a node to the vertice in the graph. This node will be the head. Each patient
     that checks in will be it's own vertice, hence it's own head.*/
    
    int i;
    all_Patients->vertice[num_Vertice].head = da_Node;
    for(i = 0; i < all_Patients->num; i++){
        if(da_Node->patient.origin == all_Patients->vertice[i].head->patient.PID){
            da_Node->patient.from = &all_Patients->vertice[i].head->patient;
            break;
        }
    }
}

void add_to_memory_to_graph(GRAPH *all_Patients, int vertices){
    /**This function allocates more mermory to store another vertice. Each patient checking in will
     correspond to a single vertice. The number of patients chacking in the number that updates
     in the graph structure*/
    
    NODE *temp_1, *temp_2;
    int i;
    int old_Size = all_Patients->num;

    all_Patients->vertice = (ROOT*)realloc(all_Patients->vertice, vertices*(sizeof(ROOT)));
    all_Patients->vertice[vertices-1].head = NULL;
    all_Patients->vertice[vertices-1].num = 1; /**Counts the first patient linked to the root node*/
    /**Assigns new number of vertices into the graph*/
    all_Patients->num = vertices;
    
}

void make_zero_node(GRAPH *all_Patients){
    /**This function assigns the first vertice in the graph, all patients, as the zero patient*/
    int zero_Patient;
    zero_Patient = 0;
    NODE *zero;
    zero = MALLOC_NEW(NODE);
    if(all_Patients->vertice[0].head == NULL){
        all_Patients->vertice[0].head = zero;
        //printf("\nZero patient equals null. Assigning structures\n");
        all_Patients->vertice[0].head->patient.PID = zero_Patient;
        all_Patients->vertice[0].head->patient.status = ZERO_PATIENT;
        all_Patients->vertice[0].head->patient.month = 99;
        all_Patients->vertice[0].head->patient.day = 99;
        all_Patients->vertice[0].head->patient.year = ZERO_PATIENT;
        all_Patients->vertice[0].head->patient.origin = zero_Patient;
    }
}

void print_graph(GRAPH *all_Patients){
    /**This Function prints the graphs vertices and the list attached to those vertices*/
    int i;
    printf("Num of vertices %d\n", all_Patients->num);
    printf("The vertices are:\n");
   
    for(i = 0; i < all_Patients->num; i++){
        printf("\tid: %5d",
               all_Patients->vertice[i].head->patient.PID);
        printf("\tdate: %2d/%2d/%2d",
               (all_Patients->vertice[i].head->patient.month), (all_Patients->vertice[i].head->patient.day), (all_Patients->vertice[i].head->patient.year));
        
        if(all_Patients->vertice[i].head->patient.status == CHECKED_IN){
            printf("\tStatus: Checked in ");
        }
        if(all_Patients->vertice[i].head->patient.status == CHECKED_OUT){
            printf("\tStatus: Checked out ");
        }
        if(all_Patients->vertice[i].head->patient.status == DECEASED){
            printf("\tStatus: Deceased ");
        }
        if(all_Patients->vertice[i].head->patient.status == ZERO_PATIENT){
            printf("\tStatus: Zero Patient");
        }
        printf("\torigin:%5d",
               all_Patients->vertice[i].head->patient.origin);
        if(all_Patients->vertice[i].head->patient.from != NULL){
            printf("\tFrom is:%5d", all_Patients->vertice[i].head->patient.from->PID);
        }
        printf("\n\t\tPatient id %d infected: ", all_Patients->vertice[i].head->patient.PID);
        print_vertice_list(&all_Patients->vertice[i]);
        printf("\n\n");
    }
}

GRAPH* make_a_graph(){
    /**This fucntion creats a graph structure*/
    GRAPH *new;
    NODE *temp_1, *temp_2;
    int i;
    new = (GRAPH*)malloc(sizeof(GRAPH));
    CHECK_MALLOC(new);
    new->num = 0; /**Makes number of vertices 0*/
    new->vertice = (ROOT*)malloc(sizeof(ROOT));
    new->vertice->head = NULL;

    return new;
}

int num_digits_in_int(int num_To_Count){
    /**This function counts the number of digits in the integer num_To_Count*/
    int count = 0;
    while(num_To_Count != 0){
        count++;
        num_To_Count /= 10;
    }
    return count;
}

float count_days(int month_1, int month_2, int day_1, int day_2,
                int year_1, int year_2){
    /**Takes in check in date and check out month and finds the difference between them*/
    int i;
    float difference;
    const int monthDays[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    float num1 = (year_1 * 365) + day_1;
    float num2 = (year_2 * 365) + day_2;
    
    for (i = 0; i < (month_1 - 1); i++){
        num1 += monthDays[i];
    }
       
    for (i = 0; i < (month_2 - 1); i++){
        num2 += monthDays[i];
    }
    
    difference = num2-num1;
    return  difference;
}
