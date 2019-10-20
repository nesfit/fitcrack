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
    :show-select="selectAll"
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
          {{ props.item.keyspace }}
        </td>
        <td class="text-right">
          {{ $moment(props.item.time_added ).format('DD.MM.YYYY HH:mm') }}
        </td>
        <td class="text-right">
          <v-tooltip top>
            <template v-slot:activator="{ on }">
              <v-btn
                icon
                class="mx-0"
                :to="{name: 'pcfgDetail', params: { id: props.item.id}}"
                disabled="true"
                v-on="on"
                @click="hideJob(props.item.id)"
              >
                <v-icon color="primary">
                  link
                </v-icon>
              </v-btn>
            </template>
            <span>Go to the PCFG page</span>
          </v-tooltip>
        </td>
      </tr>
    <!--  <h3> Items: {{ items }} </h3> -->
    </template>
  </v-data-table>
</template>

<script>
  export default {
    name: "PcfgSelector",
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
          {text: 'Keyspace', value: 'keyspace', align: 'right'},
          {text: 'Added', value: 'time_added', align: 'right'},
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
        this.axios.get(this.$serverAddr + '/pcfg').then((response) => {
          this.items = response.data.items
          this.loading = false
        })
      },
      updateSelected(id, pcfg) {
          this.selected = id
          this.$emit('input', pcfg)

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
