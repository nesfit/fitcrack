<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
    <span>
        <v-icon v-if="!keyName" @click.stop="toggle">
          {{value ? 'arrow_drop_down' : 'arrow_drop_up'}}
        </v-icon>
        <span>{</span>
        <template v-if="Object.keys(jsonValue).length">
            <json-box v-show="value" v-for="(v, k) in jsonValue" :key="k" :key-name="k" :value="v"></json-box>
            <span v-show="!value" class="node-ellipsis">...</span>
        </template>
        <span>}</span>
    </span>
</template>

<script>
export default {
    name: 'JsonObject',
    props: {
        jsonValue: Object,
        keyName: String,
        value: Boolean
    },
    methods: {
        toggle() {
            this.$emit('input', !this.value);
        }
    }
};
</script>

<style>

</style>
