<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
    <div class="node">
        <span class="key" v-if="keyName">
            <v-icon v-if="isObject"  @click.stop="toggleNode">{{value ? 'arrow_drop_down' : 'arrow_drop_up'}}</v-icon>{{keyName}}:
        </span>
        <commponent :is="`Json${valueType}`" :json-value="value" v-model="toggle" :key-name="keyName"></commponent>
    </div>
</template>

<script>
import JsonString from '@/components/json/types/json-string';
import JsonNumber from '@/components/json/types/json-number';
import JsonBoolean from '@/components/json/types/json-boolean';
import JsonObject from '@/components/json/types/json-object';
import JsonArray from '@/components/json/types/json-array';

export default {
    name: 'JsonBox',
    props: {
        value: [Object, Array, String, Number, Boolean],
        keyName: String
    },
    data() {
        return {
            toggle: true
        };
    },
    methods: {
        toggleNode() {
            this.toggle = !this.toggle;
        }
    },
    computed: {
        valueType() {
            if (Array.isArray(this.value)) {
                return 'Array';
            } else if (this.value === null){
              return 'String'
            } else if (typeof this.value === 'object') {
                return 'Object';
            } else if (typeof this.value === 'number') {
                return 'Number';
            } else if (typeof this.value === 'string') {
                return 'String';
            } else if (typeof this.value === 'boolean') {
                return 'Boolean';
            }
        },
        isObject() {
            return this.valueType == 'Array' || this.valueType == 'Object'; // eslint-disable-line
        }
    },
    components: {
        JsonString,
        JsonNumber,
        JsonBoolean,
        JsonObject,
        JsonArray
    }
};
</script>

<style lang="scss">
.node {
    font-size: 14px;
    position: relative;
    color: #525252;
    font-family: Consolas,Menlo,Courier,monospace;
    white-space: nowrap;

    &:after {
        content: ','
    }
    &:last-of-type {
        &:after {
            content: ''
        }
    }

    & .node {
        margin-left: 25px;
    }

}
</style>
