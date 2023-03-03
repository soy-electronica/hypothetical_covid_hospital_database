# hypothetical_covid_hospital_database
The implementation for the following C program was a final project for an electrical &amp; computer engineering undergraduate class in intermediate programming at the Univeristy of Maryland College Park. The project involves a hypothetical hospital and patient database. 


//** The following code takes in input file with either one of the following patient's information: (1) the date a patient checked into the hospital, (2) the date a patient checked out of the hospital, or (3) the deceased date of a patient. The date format in the input file is MM/DD/YY.  The input file also contains a given patient ID (PID) number in addition to another patient’s ID number associated. The additional PID is the who infected that given patient. If the patient does not know who infected them, the additional patient ID will be set as 0. The output file of the code is the user's input query to the database. The information in the input file for each patient is: <date> <PID1> <status> <PID2>. The "date" is the date in which the patient was given some given "status": checked in, checked out, or deceased. **//

An input example file may look like: 
  
    01/01/2000 619723 check in 0
    01/15/2020 961347 check in 0
    01/22/2020 961347 check out
    01/23/2020 257278 check in 0
    01/25/2020 619723 deceased
    02/07/2020 146521 check in 0
    02/08/2020 257278 deceased
    03/16/2020 764057 check in 0
    03/24/2020 764057 check out
    04/05/2020 1592 check in 0
    04/10/2020 896913 check in 1592
    04/15/2020 1592 check out
    04/21/2020 896913 deceased
    04/27/2020 745399 check in 0
    05/02/2020 719673 check in 745399
    05/28/2020 719673 deceased
    06/06/2020 448961 check in 0
    06/15/2020 601007 check in 0
    06/28/2020 146521 check out
    07/09/2020 448961 deceased
    07/22/2020 844669 check in 0
    07/28/2020 807869 check in 601007
    08/05/2020 807869 check out
    08/06/2020 844669 check out
    08/13/2020 623788 check in 601007
    08/14/2020 662873 check in 0
    08/23/2020 222746 check in 601007
    08/27/2020 623788 check out
    09/13/2020 585796 check in 222746
    09/27/2020 879236 check in 585796
    10/06/2020 657401 check in 601007
    10/14/2020 46324 check in 0
    10/17/2020 859042 check in 46324
    11/22/2020 171033 check in 585796
    11/22/2020 691235 check in 222746
    11/24/2020 328306 check in 601007
    11/27/2020 453697 check in 171033
    12/28/2020 54602 check in 657401
    12/28/2020 426542 check in 0
    12/28/2020 171660 check in 46324
    12/29/2020 906608 check in 222746
    12/29/2020 931025 check in 171660
    12/29/2020 950383 check in 585796
    12/29/2020 735003 check in 906608
    12/29/2020 969532 check in 879236
    12/29/2020 367565 check in 453697
    12/29/2020 601175 check in 691235
    12/29/2020 7539 check in 969532
    
The following files were used as test cases: 
[input3.txt](https://github.com/soy-electronica/hypothetical_covid_hospital_database/files/10884243/input3.txt)
[input2.txt](https://github.com/soy-electronica/hypothetical_covid_hospital_database/files/10884245/input2.txt)
