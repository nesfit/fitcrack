<!--
   * Author : Jiri Mladek
   * Licence: MIT, see LICENSE
-->

<template>
    <v-container>
        <v-sheet outlined color="grey lighten-1" rounded>
            <v-card>
                <v-card-title class="custom_grey">
                    Rules in a file:
                    <v-spacer></v-spacer>
                    <v-text-field v-model="search" append-icon="mdi-magnify" label="Search" single-line
                        hide-details></v-text-field>

                </v-card-title>
                <v-divider></v-divider>
                <v-data-table :page="lastOrCurrentPage" :options.sync="options" :headers="headers" :items="computedRules"
                    hide-default-header :search="search"
                    :footer-props="{ itemsPerPageOptions: [5, 10, 15, 20, 100, 200], itemsPerPageText: 'Rules per page', showFirstLastPage: true }">
                    <template v-slot:body="{ items }">
                        <tbody>
                            <tr v-for="item in items" :key="item.index">

                                <td>
                                    {{ item.index + 1 }}
                                </td>
                                <td class="my-0 ruleInputLine">
                                    <transition name="toolbar-fade">
                                        <div class="quickFunctionsMenuPopup mb-5" v-if="item.popupVisible">
                                            <quickFunctionsMenu :ruleIndex="item.index"
                                                :ruleLineRef="$refs['rule-' + item.index]"
                                                v-on:show-insert-popup="showInsertPopup"
                                                v-on:show-all-functions-popup="showAllFunctionsPopup"></quickFunctionsMenu>
                                        </div>
                                    </transition>
                                    <v-text-field @focus="showPopup(item.index)" @blur="hidePopup(item.index)"
                                        :ref="'rule-' + item.index" placeholder="Enter rule" autocomplete="off" hide-details
                                        outlined dense v-model="item.rule.value"
                                        @input="updateRules(item.rule.value, item.index)">
                                    </v-text-field>
                                </td>
                                <td>
                                    <v-btn v-if="item.rule.error" icon class="disableClickEffect">
                                        <v-icon color="primary">mdi-alert</v-icon>
                                    </v-btn>
                                </td>
                                <td>
                                    <v-btn icon class="px-0" color="black" @click="deleteRule(item.index)">
                                        <v-icon color="error">
                                            mdi-delete-outline
                                        </v-icon>
                                    </v-btn>
                                </td>
                            </tr>
                        </tbody>
                    </template>
                </v-data-table>
            </v-card>
        </v-sheet>
    </v-container>
</template>
  
<script>
import quickFunctionsMenu from '@/components/rule/mainEditWindow/popups/quickFunctionsMenu.vue';

export default {
    props: {
        rules: Array // array of rules {value, error}
    },
    data() {
        return {
            search: "", // for storing current search 
            options: { // default table page settings
                itemsPerPage: 5,
                page: 1
            },
            prevcomputedRulesLength: 0, // for checking if line was deleted
            goOnLastPage: false, // boolean to control if last page of table should be shown
            headers: [ // headers for the table, important for searching
                { text: "Rule name", value: "rule.value", align: "right" }
            ],
            quickFunctionsMenuVisible: false, // boolean indicating if quick functions menu should be shown
        }
    },
    methods: {
        /**
         * Method which updates specific rule
         * @param {String} newRuleValue Updated specific rule
         * @param {Number} index Index of the updating rule
         */
        updateRules(newRuleValue, index) {
            let updatedRules = this.rules;
            updatedRules[index].value = newRuleValue;
            this.$emit("update-rules", updatedRules);
        },
        /**
         * Method to delete specific rule from array
         * @param {Number} index Index of rule to be deleted
         */
        deleteRule(index) {
            let updatedRules = this.rules;
            updatedRules.splice(index, 1);
            this.$emit("update-rules", updatedRules) // update in parent
        },
        /**
         * Method which shows quick functions popup for specific rule
         * @param {Number} index Index of rule
         */
        showPopup(index) {
            this.computedRules[index].popupVisible = true;
        },
        /**
         * Method which hides quick functions popup for specific rule
         * @param {Number} index Index of rule
         */
        hidePopup(index) {
            this.computedRules[index].popupVisible = false;
        },
        /**
         * Method which propagates showing insert popup to parent
         * @param {Object} popupData Data for Insert popup to be updated 
         */
        showInsertPopup(popupData) {
            this.$emit("show-insert-popup", popupData)
        },
        /**
         * Method which propagates showing all functions popup to parent
         * @param {Object} popupData Data for all functions popup to be updated 
         */
        showAllFunctionsPopup(popupData) {
            this.$emit("show-all-functions-popup", popupData)
        }
    },
    computed: {
        /**
         * Give each rule an index and boolean which indicates if quick functions popup should be shown
         */
        computedRules() {
            return this.rules.map((rule, index) => ({
                rule,
                index,
                popupVisible: false
            }));
        },
        /**
         * Return number of current page when deleting rules, otherwise number of last page 
         */
        lastOrCurrentPage() {
            if (this.computedRules.length > this.prevcomputedRulesLength || this.goOnLastPage) {
                this.prevcomputedRulesLength = this.computedRules.length;
                return Math.ceil(this.computedRules.length / this.options.itemsPerPage);
            }
            else {
                this.prevcomputedRulesLength = this.computedRules.length;
                return this.options.page;
            }
        }
    },
    watch: {
        // when itemsPerPage value is changed, go onto the last page
        'options.itemsPerPage': function (newVal, oldVal) {
            if (newVal !== oldVal) {
                this.goOnLastPage = true;
                this.options.page = this.lastOrCurrentPage;
                this.goOnLastPage = false;
            }
        },

    },
    components: {
        quickFunctionsMenu
    }
}
</script>



<style>
.quickFunctionsMenuPopup {
    position: absolute;
    bottom: 60%;
    z-index: 9999;
    max-width: 120%;

}

.ruleInputLine {
    width: 100%;
    padding-left: 10px;
    position: relative;

}

.v-data-table>.v-data-table__wrapper>table>tbody>tr>td {
    padding: 0px 0px 0px 8px;
}

.v-data-table>.v-data-table__wrapper {
    overflow: visible;
}

.disableClickEffect {
    pointer-events: none
}

.toolbar-fade-enter,.toolbar-fade-leave-to {
    opacity: 0;
    transform: translateY(1em);
}

.toolbar-fade-enter-active {
    transition: .35s cubic-bezier(0.19, 1, 0.22, 1);
}

.toolbar-fade-leave-active {
    transition: .15s .1s ease-in;
}</style>