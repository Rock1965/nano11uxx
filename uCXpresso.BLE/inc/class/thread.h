/*
 ===============================================================================
 Name        : thread.h
 Author      : uCXpresso
 Version     : 1.1.0
 Date		 : 2014/1/5
 Copyright   : Copyright (C) www.embeda.com.tw
 Description :
 ===============================================================================
 	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2011/12/18	v1.0.0	First Edition									 Jason
 2012/04/28 v1.0.1	Remove autoDestory								 Jason
 2012/05/26 v1.0.2	add static member fuction. suspendAll,resumeAll  Jason
 2012/10/06 v1.0.3	Set virtual inheritance from CObject			 Jason
 2012/12/28 v1.0.4	add task status. (add for freeRTOS v7.3.0)		 Jason
 2013/05/31 v1.0.5  add isAlive() and kill() member functions		 Jason
 2013/07/05	v1.0.6	override the isThread() from CObject		 	 Jason
 2014/1/5	v1.0.7	reduce PRI_HARDWARE value to 4					 Jason
 2014/11/15	v1.1.0	Add task_handle to replace the run()			 Jason
  ===============================================================================
 */

#ifndef THREAD_H_
#define THREAD_H_

#include "class/object.h"

/**The RTOS group is power by FreeRTOS
 * \defgroup RTOS
 */

/**Task priorities define
 * \ingroup Enumerations
 */
typedef enum {
	PRI_LOW = 0,	///< low priorities is for all normal tasks used.
	PRI_MEDIUM,		///< medium
	PRI_HIGH,		///< high
	PRI_SUPER,		///< super
	PRI_HARDWARE	///< for hardware interrupt
}PRIORITIES_T;

/**Task states returned by eTaskStateGet.
 * \ingroup Enumerations
 */
typedef enum
{
	RUNNING = 0,	///< A task is querying the state of itself, so must be running.
	READY,			///< The task being queried is in a read or pending ready list.
	BLOCKED,		///< The task being queried is in the Blocked state.
	SUSPENDED,		///< The task being queried is in the Suspended state, or is in the Blocked state with an infinite time out.
	DELETED			///< The task being queried has been deleted, but its TCB has not yet been freed.
}TASK_STATE_T;

#define DEF_THREAD_STACK	72

/**The CThread class provide the multi-tasking services.
 * \note The CThread class is an abstract class, child class have to implement the run() member.
 * \class CThread thread.h "class/thread.h"
 * \ingroup RTOS
 */
class CThread: virtual public CObject {
	/**@brief Health Thermometer Service event handler type. */
	typedef void (*task_handle_t) (CThread * p_thread, xHandle p_param);

public:
	/**
	 * @brief CThread constructor.
	 * @param p_task_handle Pointer to the task function @ref task_handle_t.
	 * @param p_param		Pointer to the parameter of task function.
	 *
	 * @note The run() member will be called when thread start, if task_handle is null. (default)
	 */
	CThread(task_handle_t task_handle=NULL, xHandle p_param=NULL);

	/**Call the member function to start the thread.
	 * \param name is a descriptive name for the task.
	 * \param stack is a integer value to specified as the number of stack can hold-not the number of bytes.
	 * \param priority at which the task should run.
	 * \return true if successful; otherwise, crate task failed.
	 *
	 * \code
	 * Example:
	 * 		class CLedTask: public CThread {
	 * 		protected:
	 * 			virtual void run() {
	 * 				CPin led(LED2);
	 * 				while( isAlive() ) {	// Is thread in alive?  (when thread be kill, the isAlive will return false)
	 * 					led = !led;
	 * 					sleep(200);
	 * 				}
	 * 			}
	 * 		};
	 *
	 * 		int main() {
	 * 			...
	 * 			CLedTask ledTask;
	 * 			ledTask.start("led");	// default stack=128, default priority=low
	 * 			...
	 * 		}
	 * \endcode
	 */
	virtual bool start(const char *name, int stack=DEF_THREAD_STACK, PRIORITIES_T priority=PRI_LOW);

	/**Call the member function to start the thread.
	 * \note the start() is an overload member function of CThread.
	 */
	virtual bool start();

	/**Call the member function to resume the thread.
	 *
	 * \code
	 * Example:
	 *		int main() {
	 *			...
	 *			CLedTask ledTask;
	 *			ledTask.start("led");
	 *			...
	 *			ledTask.suspend();		// suspend the ledTask
	 *			...
	 *			if ( ledTask.isSuspend() ) {
	 *				ledTask.resume();	// resume the ledTask
	 *			}
	 *			...
	 *		}
	 * \endcode
	 */
	virtual void resume();

	/**Call the member function to suspend the thread.
	 *
	 * \code
	 * Example:
	 *		int main() {
	 *			...
	 *			CLedTask ledTask;
	 *			ledTask.start("led");
	 *			...
	 *			ledTask.suspend();		// suspend the ledTask
	 *			...
	 *			if ( ledTask.isSuspend() ) {
	 *				ledTask.resume();	// resume the ledTask
	 *			}
	 *			...
	 *		}
	 * \endcode
	 */
	virtual void suspend();

	/**Call the member function to check the stack high water mark.
	 * \note Or use the shell and type 'task' to check all tasks status.
	 */
	uint32_t getStackHighWaterMark();

	/**Call the member function to check the task is in suspended or not.
	 * \return true if task in suspended. otherwise, the task in running.
	 */
	virtual bool isSuspend();

	/**Retrieve the state of thread object (task)
	 */
	TASK_STATE_T getState();

	/**Call the member function to change the task's priority.
	 * \param p is PRIORITIES_T to set a new priority for the task.
	 */
	virtual void setPriority(PRIORITIES_T p);

	/**Call the member function to get the task's priority.
	 * \return PRIORITIES_T
	 */
	virtual PRIORITIES_T  getPriority();

	/**Call the member function to retrieve the task's name.
	 */
	LPCTSTR getName();

	/**isAlive is to check the thread is in alive (for run-loop)
	 * \return false if the thread is deleted.
	 */
	virtual bool isAlive();

	/**isThread(), check the class is inherited from CThread
	 */
	virtual inline bool isThread() { return true; }

	// global static functions
public:
	/**Call the CThread::resumeAll() to resume all suspended tasks.
	 */
	static void resumeAll();

	/**Call the CThread::suspendAll() to suspend all running tasks.
	 */
	static void suspendAll();

	/**Call the CThread::enterCriticalSection() to disable interrupts.
	 */
	static void enterCriticalSection();

	/**Call the CThread::exitCriticalSection() to enable interrupts.
	 */
	static void exitCriticalSection();

	/**Suspend current task which call the static member function.
	 */
	static void suspendCurrentTask();

protected:
	/**The run member function is the task main procedure, and callback by RTOS when task start.
	 *
	 * \code
	 * Example:
	 * 		class CLedTask: public CThread {
	 * 		protected:
	 * 			virtual void run() {
	 * 				CPin led(LED2);
	 * 				while( isAlive() ) {	// check thread alive
	 * 					led = !led;
	 * 					sleep(200);
	 * 				}
	 * 			}
	 * 		};
	 *
	 * 		int main() {
	 * 			...
	 * 			CLedTask ledTask;
	 * 			ledTask.start("led");	// default stack=128, default priority=low
	 * 			...
	 * 		}
	 * \endcode
	 * \remark The run() is a pure virtual function, must implement by inheritor.
	 * \note if end the run() member function, the CThread object will be destroyed and collected.
	 */
	virtual void run() { NOTHING };


	/*! \cond PRIVATE */
public:
	~CThread();
	uint32_t	m_flag;
	task_handle_t	m_task_handle;
	xHandle			m_p_param;

private:
	xHandle	 	m_xHandle;
	static void xTaskRun(CThread *pthread);
	static void destroy(xHandle task);
	/*! \endcond */
};

/**\example /rtos/blink_thread/src/main.cpp
 * This is an example of how to use the CThread class.
 * More details about this example.
 */

#endif /* THREAD_H_ */
