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
        <td class="text-right">
          <v-tooltip top>
            <template v-slot:activator="{ on }">
              <v-btn
                icon
                class="mx-0"
                :to="{name: 'ruleDetail', params: { id: props.item.id}}"
                v-on="on"
                @click="hideJob(props.item.id)"
              >
                <v-icon color="primary">
                  link
                </v-icon>
              </v-btn>
            </template>
            <span>Go to the rule page</span>
          </v-tooltip>
        </td>
      </tr>
    </template>
  </v-data-table>
</template>

<script>
  export default {
    name: "RulesSelector",
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
          {text: 'Added', value: 'time', align: 'right'},
          {text: 'Link to', value: 'name', sortable: false, align: 'right', width: "1"}
        ]
      }
    },
    watch:{
      value: function(){
        if (this.value) {
          this.selected = this.value.id
        }
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
    /*    if(this.selected == id){
          this.selected = 0
          //TODO here goes something for JSON update (set rule to null)
        this.$emit('input', null)
      }
        else{*/
          this.selected = id
          this.$emit('input', rule)
    //    }
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
