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
            <v-data-table :page="lastOrCurrentPage" :options.sync="options" :headers="headers" :items="ruleObjects"
                hide-default-header :search="search"
                :footer-props="{ itemsPerPageOptions: [5, 10, 15, 20, 100, 200], itemsPerPageText: 'Rules per page', showFirstLastPage: true }">
                <template v-slot:body="{ items }">
                    <tbody class="telicko">
                        <tr v-for="item in items" :key="item.index">
                            <td class="other">
                                {{ item.index + 1 }}
                            </td>
                            <td class="my-0 ruleInputLine">
                                <v-text-field :id="`ruleLineField-${item.index}`" @focus="showPopup(item.index)"
                                    @blur="hidePopup(item.index)" placeholder="Enter rule" autocomplete="off" hide-details
                                    outlined dense v-model="item.rule" @input="updateRules(item.rule, item.index)">
                                </v-text-field>
                                <div class="quickFunctionsMenuPopup" v-show="item.popupVisible">
                                    <quickFunctionsMenu v-bind:ruleIndex="item.index"
                                        v-on:show-insert-popup="showInsertPopup"
                                        v-on:show-all-functions-popup="showAllFunctionsPopup"></quickFunctionsMenu>
                                </div>
                            </td>
                            <td class="other">
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
        rulesList: Array
    },
    data() {
        return {
            search: "",
            options: { //default table page settings
                itemsPerPage: 5,
                page: 1
            },
            prevRuleObjectsLength: 0, // for checking if line was deleted
            goOnLastPage: false,
            headers: [
                { text: "Rule name", value: "rule", align: "right" },
                { text: "ID", value: "id" },
            ],
            quickFunctionsMenuVisible: false,
            rulesListData: this.rulesList
        }
    },
    methods: {
        updateRules(rule, index) {
            this.rulesListData = this.rulesList;
            this.rulesListData[index] = rule;
            this.$emit("update-rules", this.rulesListData);
        },
        deleteRule(index) {
            this.rulesListData = this.rulesList
            this.rulesListData.splice(index, 1);
            this.$emit("update-rules", this.rulesListData)
        },
        showPopup(index) {
            this.ruleObjects[index].popupVisible = true;
        },
        hidePopup(index) {
            this.ruleObjects[index].popupVisible = false;
        },
        showInsertPopup(insertData) {
            this.$emit("show-insert-popup", insertData)
        },
        showAllFunctionsPopup(popupData) {
            this.$emit("show-all-functions-popup", popupData)
        }

    },
    computed: {
        ruleObjects() {
            return this.rulesList.map((rule, index) => ({
                rule,
                index,
                popupVisible: false
            }));
        },
        /**
         * Return number of current page when deleting rules, otherwise number of last page 
         */
        lastOrCurrentPage() {
            if (this.ruleObjects.length > this.prevRuleObjectsLength || this.goOnLastPage) {
                this.prevRuleObjectsLength = this.ruleObjects.length;
                return Math.ceil(this.ruleObjects.length / this.options.itemsPerPage);
            }
            else {
                this.prevRuleObjectsLength = this.ruleObjects.length;
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
        }
    },
    components: {
        quickFunctionsMenu
    }
}
</script>



<style>
.other {
    width: 5%;
}

.ruleInputLine {
    position: relative;
}

.quickFunctionsMenuPopup {
    position: absolute;
    bottom: 100%;
    max-width: 120%;
    padding: 0;
}
</style>