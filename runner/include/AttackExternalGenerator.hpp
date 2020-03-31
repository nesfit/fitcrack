#ifndef RUNNER_ATTACK_EXTERNAL_GENERATOR_HPP
#define RUNNER_ATTACK_EXTERNAL_GENERATOR_HPP

#include "AttackCrackingBase.hpp"

/** Class representing hashcat's/PFCG manager PCFG attack */
class AttackExternalGenerator: public AttackCrackingBase {
	std::vector<std::string> generator_arguments_; /**< PCFG-manager arguments based on the config */
	protected:

		/**
		 * @brief   Adds all attack specific arguments
		 */
		void addGeneratorArgument(const std::string &argument);
		void addGeneratorRequiredFile(const std::string& file_name);
		virtual void addGeneratorLimitingArgument(const std::string &argument) {addGeneratorArgument(argument);}
		virtual void addGeneratorLimitingRequiredFile(const std::string& file_name) {addGeneratorRequiredFile(file_name);}

	public:

		/**
		 * @brief   Basic constructor
		 * @param   config [in] Representation of config file
		 * @param   directory [in] Working directory
		 */
		AttackExternalGenerator(const ConfigTask &config, Directory &directory);

		virtual std::string getExternalGeneratorName() = 0;

		virtual std::vector<std::string> getExternalGeneratorArguments() const {return generator_arguments_;}
};

#endif //RUNNER_ATTACK_EXTERNAL_GENERATOR_HPP
