<template>
    <v-container>
        <v-card>
            <v-card-title class="grey lighten-2">
                Rules in a file:
                <v-spacer></v-spacer>
                <v-text-field v-model="search" append-icon="mdi-magnify" label="Search" single-line
                    hide-details></v-text-field>

            </v-card-title>
            <v-divider></v-divider>
            <v-data-table :headers="headers" :items="ruleObjects" hide-default-header :search="search"
                :footer-props="{ itemsPerPageOptions: [5, 7, 15, 20], itemsPerPageText: 'Rules per page' }">
                <template v-slot:body="{ items }">
                    <tbody>
                        <tr v-for="item in items" :key="item.index">
                            <td class="other">
                                {{ item.index + 1 }}
                            </td>
                            <td class="mainClass my-0">
                                <v-text-field placeholder="Enter rule" hide-details outlined dense v-model="item.rule"
                                    @input="updateRules(item.rule, item.index)"></v-text-field>
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

                <!--
                <tr v-for="(rule, index) in rules" :key="index">
                    <td>
                        {{ index + 1 }}
                    </td>
                    <td>
                        <v-text-field placeholder="Enter rule" hide-details outlined dense v-model="rules[index]"
                            @input="updateRules"></v-text-field>
                    </td>

                </tr>
                -->

                <!--

                <template #item="{ item, index }">
                    <tr>
                        <td>
                            {{ index + 1}}
                        </td>
                        <td>
                            <v-text-field placeholder="Enter rule" hide-details outlined dense v-model="item.item"
                                @input="updateRules"></v-text-field>
                        </td>
                        <td>
                            <v-btn icon class="px-0" color="black" @click="deleteRule(index)">
                                <v-icon color="error">
                                    mdi-delete-outline
                                </v-icon>
                            </v-btn>
                        </td>
                    </tr>
                </template>                    
                -->



            </v-data-table>
            <!--
            <v-list>
                <v-list-item v-for="(rule, index) in rules" :key="index">
                    <div class="mr-3">{{ index + 1 }}</div>

                    <v-text-field placeholder="Enter rule" hide-details outlined dense v-model="rules[index]"
                        @input="updateRules"></v-text-field>
                    <v-btn icon class="px-0" color="black" @click="deleteRule(index)">
                        <v-icon color="error">
                            mdi-delete-outline
                        </v-icon>
                    </v-btn>
                </v-list-item>
            </v-list>
        -->
        </v-card>
    </v-container>
</template>
  
<script>

export default {
    props: ["rules"],
    data() {
        return {
            search: "",
            headers: [
                { text: "Rule name", value: "rule", align: "right" },
                { text: "ID", value: "id" },
            ],
        }
    },
    methods: {
        updateRules(rule, index) {
            this.rules[index] = rule;
            this.$emit("rules-updated", this.rules);
        },
        deleteRule(index) {
            this.rules.splice(index, 1);
            this.$emit("rules-updated", this.rules)
        }
    },
    computed: {
        ruleObjects() {
            return this.rules.map((rule, index) => ({ rule, index }));
        },
    }
}
</script>



<style>
.other{
    width: 5%;
}



</style>