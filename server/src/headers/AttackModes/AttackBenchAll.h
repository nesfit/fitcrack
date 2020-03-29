/**
 * @file AttackBench.h
 * @brief Header file for creation of Benchmark workunit
 * @authors see AUTHORS
 * @date 12. 12. 2018
 * @license MIT, see LICENSE
 */
#pragma once

#include <AttackMode.h>


class CAttackBenchAll : public AttackMode {
public:
	/**
		* @brief Constructor for Benchmark Workunit
		* @param job [in] Instance of CJob which is parent of this workunit instance
		* @param host [in] Instance of CHost which this workunit belongs to
		*/
	CAttackBenchAll(PtrJob &job, PtrHost &host, CSqlLoader *sqlLoader);

	/**
		* @brief Default destructor
		*/
	~CAttackBenchAll() override = default;

	/**
		* @brief Creates BOINC workunit, adds entry to fc_workunit
		* @return True if a workunit was planned, False otherwise
		*/
	bool makeWorkunit() override ;

protected:
	virtual char getModeLetter() override {return 'a';}

private:
	/**
		* @brief Function to generate new CWorkunit for certain host
		* @return True if workunit was generated successfully, False otherwise
		*/
	bool generateWorkunit() override ;
};
