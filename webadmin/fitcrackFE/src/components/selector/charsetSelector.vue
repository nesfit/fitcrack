<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <v-data-table
    :headers="headers"
    :items="items"
    :search="search"
    v-model="selected"
    item-key="id"
    :select-all="selectAll"
    @input="updateSelected"
  >
    <template slot="items" slot-scope="props">
      <tr @click="(trySelect(props.selected) ? props.selected = !props.selected : $error('You can only select maximum of 4 charsets'))">
        <td>
          <v-checkbox
            @click="(trySelect(props.selected) ? props.selected = !props.selected : $error('You can only select maximum of 4 charsets'))"
            v-model="props.selected"
            primary
            hide-details
          ></v-checkbox>
        </td>
        <td>{{ props.item.name }}</td>
        <td class="text-xs-right">{{ $moment(props.item.time ).format('DD.MM.YYYY HH:mm') }}</td>
        <td class="text-xs-right">
          <v-tooltip top>
            <v-btn icon class="mx-0" :to="{name: 'charsetDetail', params: { id: props.item.id}}" slot="activator">
              <v-icon color="primary">link</v-icon>
            </v-btn>
            <span>Go to the charset page</span>
          </v-tooltip>
        </td>
      </tr>
    </template>
  </v-data-table>
</template>

<script>
  export default {
    name: "charsetSelector",
    props: {
      selectAll: {
        type: Boolean,
        default: false
      },
      value: {
        type: Array,
        default: function () {
          return []
        }
      }
    },
    data() {
      return {
        items: [],
        loading: false,
        search: '',
        selected: [],
        headers: [
          {
            text: 'Name',
            align: 'left',
            value: 'name'
          },
          {text: 'Added', value: 'time', align: 'right'},
          {text: 'Link to', value: 'name', sortable: false, align: 'right', width: "1"}
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
        this.axios.get(this.$serverAddr + '/charset').then((response) => {
          this.items = response.data.items
          this.loading = false
        })
      },
      updateSelected() {
        this.$emit('input', this.selected)
      },
      trySelect(isSelected) {
        return isSelected || this.selected.length < 4
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
