<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
    <div class="json-viewer">
        <div class="code-box" >
            <json-box :value="value"></json-box>
        </div>

    </div>
</template>

<script>
import Vue from 'vue';
import JsonBox from '@/components/json/json-box'
Vue.component(JsonBox.name, JsonBox);
export default {
    name: 'JsonViewer',
    props: {
        value: [Object, Array, String, Number, Boolean]
    },
     components: {
        JsonBox
    }
};
</script>

<style lang="scss">
.json-viewer {
    box-sizing: border-box;
    position: relative;

    .code-box {
        padding: 20px;
    }

}
</style>
