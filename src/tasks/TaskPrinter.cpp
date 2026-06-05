#include "tasks/TaskPrinter.h"
#include "core/Queue.h"
#include "io/Printer.h"
#include "core/StateMachine.h"
#include "core/RuntimeStorage.h"
//#include "core/PrinterManager.h"
#include "core/PrintJob.h"

extern Printer printer;

void TaskPrinter(void *pv)
{
    PrintJob job;
    printer.begin();
    while (1) {

       
        if (PrintQueue_Receive(&job, portMAX_DELAY)) 
        {
            
            if (job.type == PRINT_TRANSACTION)
            {
                printer.printTransactionSlip(job.trx);
            }

            if (job.type == PRINT_REPRINT)
            {
                printer.reprintLastTransaction(job.trx);
            }

            if (job.type == PRINT_SHIFTREPORT)
            {
                printer.printCloseShift(job.shift);
            }

            if (job.type == PRINT_CURRENTSHIFT)
            {   
                printer.printCurrentShift(job.shift);
            }
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

// Pendeketan dengan shared flag system
// void TaskPrinter(void *pv)
// {
//     printer.begin();
//     while (1) {

//         if (PrinterManager::isPrintRequested()) {

//             //Serial.println("PRINT REQUEST RECEIVED");

//             TransactionData trx;

//             RuntimeStorage::Transaction_GetData(trx);

//             printer.printTransactionSlip(trx);

//             PrinterManager::clearPrintRequest();
//         }

//          if (PrinterManager::isRePrintRequested()) {

//             //Serial.println("PRINT REQUEST RECEIVED");

//             TransactionData trx;

//             RuntimeStorage::Transaction_GetLast(trx);

//             printer.reprintLastTransaction(trx);

//             PrinterManager::clearRePrintRequest();
//         }


//         if (PrinterManager::isPrintCurrentShiftRequested()) {

//             //Serial.println("PRINT CURRENT SHIFT REQUEST RECEIVED");

//             ShiftData shift;

//             RuntimeStorage::Shift_GetData(shift);
//             printer.printCurrentShift(shift);

//             PrinterManager::clearPrintCurrentShiftRequest();
//         }

//              if (PrinterManager::isPrintCloseShiftRequested()) {

//             //Serial.println("PRINT CURRENT SHIFT REQUEST RECEIVED");
//             ShiftData shift;

//             RuntimeStorage::Shift_GetData(shift);

//             printer.printCloseShift(shift);

//             PrinterManager::clearPrintCloseShiftRequest();
//         }
    
//     }
// }