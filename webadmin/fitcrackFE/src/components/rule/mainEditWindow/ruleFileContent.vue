<template>
    <v-container>
        <v-card>
            <v-card-title class="hovno grey lighten-2">
                Rules in a file:
                <v-spacer></v-spacer>
                <v-text-field v-model="search" append-icon="mdi-magnify" label="Search" single-line
                    hide-details></v-text-field>

            </v-card-title>
            <v-divider></v-divider>
            <v-data-table :headers="headers" :items="ruleObjects" hide-default-header :search="search"
                :footer-props="{ itemsPerPageOptions: [5, 7, 15, 20], itemsPerPageText: 'Rules per page' }">
                <template v-slot:body="{ items }">
                    <!--
                    <v-row v-show="quickFunctionsMenuVisible" ref="popup">
                        <quickFunctionsMenu></quickFunctionsMenu>
                    </v-row>
                    -->
                    <tbody>
                        <tr v-for="item in items" :key="item.index" :id="`ruleLine-${item.index}`">

                            <td class="other">
                                {{ item.index + 1 }}
                            </td>
                            <td key class="my-0 ruleInputLine">
                                <v-sheet :elevation="10" class="quickFunctionsMenuPopup" v-show="quickFunctionsMenuVisible"
                                    ref="popup">
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
                                <v-text-field @focus="showPopup()" @blur="hidePopup()" placeholder="Enter rule" hide-details
                                    outlined dense v-model="item.rule"
                                    @input="updateRules(item.rule, item.index)">
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
            popupTop: 0,
            popupLeft: 0
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
        showPopup() {
            console.log("clicked")
            /*
            const rowElement = document.querySelectorAll(".ruleInputLine")[1]
            const { top, left } = rowElement.getBoundingClientRect()
            //console.log(top, left)

            // Set the position of the popup element
            this.popupTop = top
            this.popupLeft = left

            console.log(this.$refs)
            this.$refs.popup.style.top = `${this.popupTop}px`;
            this.$refs.popup.style.left = `${this.popupLeft}px`;

            */
            // Show the popup element
            this.quickFunctionsMenuVisible = true
        },
        hidePopup() {
            this.quickFunctionsMenuVisible = false;
        }

    },
    computed: {
        ruleObjects() {
            return this.rulesList.map((rule, index) => ({ rule, index }));
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
    background: black;
    z-index: 10;
    bottom: 110%;
    max-width: 100%;
    user-select: none;
    border-radius: 1.3em;
    padding: 0;
}
</style>