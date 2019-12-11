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
    show-select
    :single-select="!selectAll"
    @input="updateSelected"
  >
    <template v-slot:item.name="{ item }">
      <router-link :to="{name: 'ruleDetail', params: { id: item.id}}">
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
    name: "RulesSelector",
    mixins: [selector],
    props: {
      value: {
        type: Object,
        default: () => ({})
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
        ]
      }
    },
    watch:{
      value: function(){
        if (this.value) {
          this.selected = this.value
        }
      }
    },
    methods: {
      getData() {
        this.loading = true
        this.axios.get(this.$serverAddr + '/rule').then((response) => {
          this.items = response.data.items
          this.loading = false
        })
      },
      updateSelected(rule) {
    /*    if(this.selected == id){
          this.selected = 0
          //TODO here goes something for JSON update (set rule to null)
        this.$emit('input', null)
      }
        else{*/
          //this.selected = id
          this.$emit('input', this.selected[0])
    //    } WHAT??
      }
    },
  }
</script>