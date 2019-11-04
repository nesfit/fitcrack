<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <fc-tile title="Dictionaries and rules">
    <v-list
      single-line
      class="width100"
    >
      <v-data-table
        :headers="headers"
        :items="data.left_dictionaries"
        item-key="id"
        hide-default-footer
      >
        <template
          slot="items"
          slot-scope="props"
        >
          <td>{{ props.item.dictionary.name }}</td>
          <td class="text-right">
            {{ props.item.dictionary.keyspace }}
          </td>
          <td class="text-right">
            {{ $moment(props.item.dictionary.time ).format('DD.MM.YYYY HH:mm') }}
          </td>
          <td class="text-right">
            <v-tooltip top>
              <template v-slot:activator="{ on }">
                <v-btn
                  icon
                  class="mx-0"
                  :to="{name: 'dictionaryDetail', params: { id: props.item.dictionary.id}}"
                  v-on="on"
                >
                  <v-icon color="primary">
                    link
                  </v-icon>
                </v-btn>
              </template>
              <span>Go to the dictionary page</span>
            </v-tooltip>
          </td>
        </template>
      </v-data-table>
      <template v-if="data.rulesFile.id !== null">
        <v-divider />
        <v-list-item class="px-2 py-1">
          <v-list-item-action class="pr-3 key">
            Rules:
          </v-list-item-action>
          <v-list-item-content>
            <v-list-item-title class="text-right">
              <router-link :to="{name: 'ruleDetail', params: { id: data.rulesFile.id}}">
                {{ data.rulesFile.name }}
              </router-link>
            </v-list-item-title>
          </v-list-item-content>
        </v-list-item>
      </template>
    </v-list>
  </fc-tile>
</template>

<script>
  import tile from '@/components/tile/fc_tile'
  export default {
    name: "DictionaryDetail",
    components: {
      'fc-tile': tile
    },
    props: ['data'],
    data() {
      return {
        headers: [
          {
            text: 'Name',
            align: 'left',
            value: 'name'
          },
          {text: 'Keyspace', value: 'keyspace', align: 'right'},
          {text: 'Time', value: 'time', align: 'right'},
          {text: 'Link to', value: 'name', sortable: false, align: 'right', width: "1"}
        ]
      }
    },
  }
</script>

<style scoped>

</style>
