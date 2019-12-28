<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <v-data-table
    :value="selected"
    :headers="headers"
    :items="items"
    :search="search"
    item-key="id"
    show-select
    @input="updateSelected"
  >
    <template v-slot:item.name="{ item }">
      <router-link :to="{name: 'charsetDetail', params: { id: item.id}}">
        {{ item.name }}
        <v-icon 
          small
          color="primary"
        >
          mdi-open-in-new
        </v-icon>
      </router-link>
    </template>
    <template v-slot:item.time="{ item }">
      {{ $moment(item.time).format('DD.MM.YYYY HH:mm') }}
    </template>
  </v-data-table>
</template>

<script>
  import selector from './selectorMixin'
  export default {
    name: "CharsetSelector",
    mixins: [selector],
    data() {
      return {
        headers: [
          {
            text: 'Name',
            align: 'start',
            value: 'name'
          },
          {text: 'Added', value: 'time', align: 'end'},
        ],
      }
    },
    methods: {
      getData() {
        this.loading = true
        this.axios.get(this.$serverAddr + '/charset').then((response) => {
          this.items = response.data.items
          this.loading = false
        })
      },
      updateSelected(arr) {
        if (arr.length > 4) arr = arr.slice(1).slice(-4)
        this.selected = arr
        this.$emit('input', arr)
      }
    },
  }
</script>

<style scoped>
  .oneline {
    white-space: nowrap;
    overflow: hidden;
    text-overflow: ellipsis;
    display: block;
    width: 200px;
    vertical-align: middle;
    line-height: 50px;
    height: 50px;
  }
</style>
