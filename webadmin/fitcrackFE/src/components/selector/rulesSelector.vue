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
    <template slot="items" slot-scope="props">
      <tr @click="updateSelected(props.item.id, props.item)">
        <td>
          <v-checkbox
            :input-value="selected === props.item.id"
            @click="updateSelected(props.item.id, props.item)"
            primary
            hide-details
          ></v-checkbox>
        </td>
        <td>{{ props.item.name }}</td>
        <td class="text-xs-right">{{ $moment(props.item.time ).format('DD.MM.YYYY HH:mm') }}</td>
        <td class="text-xs-right">
          <v-tooltip top>
            <v-btn icon class="mx-0" :to="{name: 'ruleDetail', params: { id: props.item.id}}" slot="activator" @click="hideJob(props.item.id)">
              <v-icon color="primary">link</v-icon>
            </v-btn>
            <span>Go to the rule page</span>
          </v-tooltip>
        </td>
      </tr>
    </template>
  </v-data-table>
</template>

<script>
  export default {
    name: "rulesSelector",
    props: {
      selectAll: {
        type: Boolean,
        default: false
      },
      value: {
        type: Object
      }
    },
    watch:{
      value: function(){
        if (this.value) {
          this.selected = this.value.id
        }
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
          {text: 'Added', value: 'time', align: 'right'},
          {text: 'Link to', value: 'name', sortable: false, align: 'right', width: "1"}
        ]
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
        this.axios.get(this.$serverAddr + '/rule').then((response) => {
          this.items = response.data.items
          this.loading = false
        })
      },
      updateSelected(id, rule) {
        this.selected = id
        this.$emit('input', rule)
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
