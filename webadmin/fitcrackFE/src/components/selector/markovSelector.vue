<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <v-data-table
    :headers="headers"
    :items="items"
    :search="search"
    item-key="id"
    :select-all="selectAll"
  >
    <template
      slot="items"
      slot-scope="props"
    >
      <tr>
        <td>
          <v-checkbox
            :input-value="selected === props.item.id"
            primary
            hide-details
            @click="updateSelected(props.item.id, props.item)"
          />
        </td>
        <td>{{ props.item.name }}</td>
        <td class="text-right">
          {{ $moment(props.item.time ).format('DD.MM.YYYY HH:mm') }}
        </td>
      </tr>
    </template>
  </v-data-table>
</template>

<script>
  export default {
    name: "MarkovSelector",
    props: {
      selectAll: {
        type: Boolean,
        default: false
      },
      value: {
        type: Object
      }
    },
    data() {
      return {
        items: [],
        loading: false,
        search: '',
        selected: 0,
        headers: [
          {
            text: 'Name',
            align: 'left',
            value: 'name'
          },
          {text: 'Added', value: 'time', align: 'right'}
        ],
      }
    },
    mounted() {
      if (!this.selectAll) {
        this.headers.unshift({width: "1"})
      }
      this.getData()
    },
    methods: {
      getData() {
        this.loading = true
        this.axios.get(this.$serverAddr + '/markovChains').then((response) => {
          this.items = response.data.items
          this.loading = false
        })
      },
      updateSelected(id, markov) {
        this.selected = id
        this.$emit('input', markov)
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
