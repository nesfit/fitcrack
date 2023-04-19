<template>
    <v-container>
        <v-card style="z-index: 1;">
            <v-card-title class="v-card-title-class grey lighten-2">
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
                    <tbody class="telicko">
                        <tr v-for="item in items" :key="item.index">
                            <td>
                                {{ item.index + 1 }}
                            </td>
                            <td class="my-0 ruleInputLine">
                                <v-text-field @focus="showPopup(item.index)" @blur="hidePopup(item.index)"
                                    @keydown="getCursorPosition($event, item.index)" @click="getCursorPosition($event, item.index)" :ref="'rule-' + item.index"
                                    placeholder="Enter rule" autocomplete="off" hide-details outlined dense
                                    v-model="item.rule.value" @input="updateRules(item.rule.value, item.index)">
                                </v-text-field>
                                <div class="quickFunctionsMenuPopup" v-show="item.popupVisible">
                                    <quickFunctionsMenu v-bind:ruleIndex="item.index"
                                        v-on:show-insert-popup="showInsertPopup"
                                        v-on:show-all-functions-popup="showAllFunctionsPopup"></quickFunctionsMenu>
                                </div>
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
    </v-container>
</template>
  
<script>
import quickFunctionsMenu from '@/components/rule/mainEditWindow/popups/quickFunctionsMenu.vue';

export default {
    props: {
        rules: Array
    },
    data() {
        return {
            search: "",
            options: { //default table page settings
                itemsPerPage: 5,
                page: 1
            },
            prevcomputedRulesLength: 0, // for checking if line was deleted
            goOnLastPage: false,
            headers: [
                { text: "Rule name", value: "rule", align: "right" },
                { text: "ID", value: "id" },
            ],
            quickFunctionsMenuVisible: false,
        }
    },
    methods: {
        updateRules(newRuleValue, index) {
            let updatedRules = this.rules;
            updatedRules[index].value = newRuleValue;
            this.$emit("update-rules", updatedRules);
        },
        deleteRule(index) {
            let updatedRules = this.rules;
            updatedRules.splice(index, 1);
            this.$emit("update-rules", updatedRules)
        },
        showPopup(index) {
            this.computedRules[index].popupVisible = true;
        },
        hidePopup(index) {
            this.computedRules[index].popupVisible = false;
        },
        showInsertPopup(insertData) {
            this.$emit("show-insert-popup", insertData)
        },
        showAllFunctionsPopup(popupData) {
            this.$emit("show-all-functions-popup", popupData)
        },
        getCursorPosition(event, ruleIndex) {
            console.log(event)
            const inputElement = this.$refs["rule-" + ruleIndex][0].$el.querySelector('input');
            const cursorPosition = inputElement.selectionStart;
            console.log(cursorPosition) //TODO

        }

    },
    computed: {
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
        // When itemsPerPage value is changed, go onto the last page
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
tr {
    padding: 5px;
}

.v-data-table>.v-data-table__wrapper>table>tbody>tr>td {
    padding: 0px 0px 0px 5px;
}

.v-data-table>.v-data-table__wrapper>table>tbody>tr>td.ruleInputLine {
    position: relative;
    width: 100%;
    padding-left: 10px;
}

.quickFunctionsMenuPopup {
    position: absolute;
    bottom: 100%;
    max-width: 120%;
    padding: 0;
}

.disableClickEffect {
    pointer-events: none
}
</style>