<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <v-data-table
    v-model="selected"
    :headers="headers"
    :items="items"
    :search="search"
    item-key="id"
    :show-select="markovSubmode != 0"
    :single-select="!selectAll"
    @input="updateSelected"
  >
    <template v-slot:item.time="{ item }">
      {{ $moment(item.time).format('DD.MM.YYYY HH:mm') }}
    </template>
  </v-data-table>
</template>

<script>
  import selector from './selectorMixin'
  export default {
    name: "MarkovSelector",
    mixins: [selector],
    props: {
      value: {
        type: Object,
        default: () => ({})
      },
      markovSubmode: {
        type: Number,
        default: 2
      }
    },
    data() {
      return {
        headers: [
          {
            text: 'Name',
            align: 'start',
            value: 'name'
          },
          {text: 'Added', value: 'time', align: 'end'}
        ],
      }
    },
    methods: {
      getData() {
        this.loading = true
      //  this.markovSubmode = 0
        this.axios.get(this.$serverAddr + '/markovChains').then((response) => {
          this.items = response.data.items
          this.loading = false
        })
      }
    }
  }
</script>
