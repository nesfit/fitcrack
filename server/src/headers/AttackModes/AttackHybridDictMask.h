#ifndef FITCRACK_HEADER_ATTACK_HYBRID_DICTMASK_H_
#define FITCRACK_HEADER_ATTACK_HYBRID_DICTMASK_H_

#include <AttackMode.h>

class CAttackHybridDictMask : public AttackMode {
	public:
		/**
		 * @brief Constructor for Combinator Attack
		 * @param job [in] Instance of CJob which is parent of this attack instance
		 * @param host [in] Instance of CHost which this attack belongs to
		 * @param seconds [in] Number of seconds this instance of attack should take
		 */
		CAttackHybridDictMask(PtrJob job, PtrHost &host, uint64_t seconds, CSqlLoader *sqlLoader);

		/**
		 * @brief Creates BOINC workunit, adds entry to fc_workunit
		 * @return True if a workunit was planned, False otherwise
		 */
		bool makeWorkunit() override ;

		virtual bool requiresDicts() const override {return true;}
		virtual bool requiresMasks() const override {return true;}
		virtual bool masksUseRealKeyspace() const override {return true;}

	private:
		/**
		 * @brief Function to generate new CWorkunit for certain host for given time
		 * @return True if workunit was generated successfully, False otherwise
		 */
		bool generateWorkunit() override ;
};

#endif //FITCRACK_HEADER_ATTACK_HYBRID_DICTMASK_H_