<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <div>
    <v-breadcrumbs
      v-if="data != null"
      :items="
        [
          { text: 'Charsets', to: { name: 'charsets' }, exact: true },
          { text: data.name }
        ]"
      divider="/"
    />

    <v-container>
      <v-row justify="center">
        <fc-tile
          title="Charsets"
          :loading="data==null"
          class="mx-2 dictContentContainer mb-4"
        >
          <v-list
            v-if="data != null"
          >
            <v-list-item>
              <v-list-item-action>
                Name:
              </v-list-item-action>
              <v-list-item-content>
                <v-list-item-title class="text-right">
                  {{ data.name }}
                </v-list-item-title>
              </v-list-item-content>
            </v-list-item>
            <v-divider />
            <v-list-item>
              <v-list-item-action>
                Added:
              </v-list-item-action>
              <v-list-item-content>
                <v-list-item-title class="text-right">
                  {{ $moment(data.time).format('DD.MM.YYYY HH:mm') }}
                </v-list-item-title>
              </v-list-item-content>
            </v-list-item>
          </v-list>
          <v-row class="mx-2">
            <v-col>
              <v-btn
                v-if="!edit"
                outlined
                color="primary"
                @click="edit=true"
              >
                Edit <v-icon right>
                  mdi-pencil
                </v-icon>
              </v-btn>
              <v-btn
                v-else
                outlined
                color="primary"
                @click="saveCharset"
              >
                Save 
                <v-icon right>
                  mdi-content-save
                </v-icon>
              </v-btn>
              <v-btn
                class="ma-0"
                outlined
                color="primary"
                :href="$serverAddr + '/charset/' + data.id + '/download'"
                target="_blank"
              >
                Download <v-icon right>mdi-download</v-icon>
              </v-btn>
            </v-col>
          </v-row>
          <v-divider />
          <div
            v-if="data != null"
            class="dictContent pa-2"
            :class="{editing: edit}"
          >
            <code
              class="code elevation-0"
              :contenteditable="edit"
              @input="update"
            >{{ data.data }}</code>
          </div>
        </fc-tile>
      </v-row>
    </v-container>
  </div>
</template>

<script>
  import tile from '@/components/tile/fc_tile'
  export default {
    name: "CharsetDetailView",
    components: {
      'fc-tile': tile
    },
    data: function () {
      return {
        data: null,
        edit: false,
        newData: ''
      }
    },
    mounted: function () {
      this.loadData()
    },
    methods: {
      loadData: function ($state) {
        this.axios.get(this.$serverAddr + '/charset/' + this.$route.params.id).then((response) => {
          this.data = response.data
          if (!this.data['canDecode'])
            this.$error('Can not decode encoding.')
        })
      },
      update:function(event){
        console.log('text updated')
        this.newData = event.target.innerText
      },
      saveCharset: function () {
        if (this.newData === '')
          return

        this.axios.post(this.$serverAddr + '/charset/' + this.data.id + '/update', {
          "newCharset": this.newData
        }).then((response) => {
          console.log(response.data)
          this.edit=false
        })
      }
    }
  }
</script>

<style scoped>
  .dictContentContainer {
    max-width: 800px;
  }
  .dictContent {
    overflow: auto;
  }

  .code::before {
    display: none;
  }
  .code {
    width: 100%;
    background: transparent;
    color: #222;
  }
  .theme--dark .code {
    color: #dedede;
  }
  .width100 {
    width: 100%;
  }

  .margintop5 {
    margin-top: 10px;
  }
</style>
