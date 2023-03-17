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
            <v-data-table class="hovnomoje" :headers="headers" :items="ruleObjects" hide-default-header
                :search="search"
                :footer-props="{ itemsPerPageOptions: [5, 7, 15, 20], itemsPerPageText: 'Rules per page' }">
                <template v-slot:body="{ items }">
                    <tbody class="telicko">
                        <tr v-for="item in items" :key="item.index">
                            <td class="other">
                                {{ item.index + 1 }}
                            </td>
                            <td class="my-0 ruleInputLine">
                                <v-sheet :elevation="10" class="quickFunctionsMenuPopup" v-show="item.popupVisible"
                                    :id="`ruleLinePopup-${item.index}`">
                                    <v-chip>
                                        <v-btn>
                                            ahokj
                                        </v-btn>
                                    </v-chip>
                                    <v-chip>
                                        <v-btn>
                                            ahokj
                                        </v-btn>
                                    </v-chip>
                                </v-sheet>
                                <v-text-field :id="`ruleLineField-${item.index}`" @focus="showPopup(item.index)"
                                    @blur="hidePopup(item.index)" placeholder="Enter rule" hide-details outlined dense
                                    v-model="item.rule" @input="updateRules(item.rule, item.index)">
                                </v-text-field>
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
    props: ["rulesList"],
    data() {
        return {
            search: "",
            headers: [
                { text: "Rule name", value: "rule", align: "right" },
                { text: "ID", value: "id" },
            ],
            quickFunctionsMenuVisible: false,
        }
    },
    methods: {
        updateRules(rule, index) {
            this.rulesList[index] = rule;
            this.$emit("rules-updated", this.rulesList);
        },
        deleteRule(index) {
            this.rulesList.splice(index, 1);
            this.$emit("rules-updated", this.rulesList)
        },
        showPopup(index) {
            console.log("clicked")
            this.ruleObjects[index].popupVisible = true;
        },
        hidePopup(index) {
            this.ruleObjects[index].popupVisible = false;
        },

    },
    computed: {
        ruleObjects() {
            return this.rulesList.map((rule, index) => ({
                rule,
                index,
                popupVisible: false
            }));
        },
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
    max-width: 100%;
    border-radius: 1.3em;
    padding: 0;
}

</style>