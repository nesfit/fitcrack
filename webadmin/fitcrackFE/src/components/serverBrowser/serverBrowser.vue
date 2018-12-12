<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <fc-tile title="Server browser" :loading="data==null || adding" class="mx-auto dictContentContainer">
    <v-list v-if="data!==null" class="filesCont">
      <v-list-tile
        @click="loadDirectory(data.parent)"
      >
        <v-list-tile-action>
          <v-icon>keyboard_backspace</v-icon>
        </v-list-tile-action>
        <v-list-tile-content>
          <v-list-tile-title>..</v-list-tile-title>
          <v-list-tile-sub-title>{{ data.parent }}</v-list-tile-sub-title>
        </v-list-tile-content>
      </v-list-tile>
      <v-list-tile
        v-for="item in data.folders"
        :key="item.name"
        @click="loadDirectory(item.path)"
      >
        <v-list-tile-action>
          <v-icon>folder</v-icon>
        </v-list-tile-action>
        <v-list-tile-content>
          <v-list-tile-title>{{ item.name }}</v-list-tile-title>
          <v-list-tile-sub-title>{{ item.path }}</v-list-tile-sub-title>
        </v-list-tile-content>
      </v-list-tile>
      <v-list-tile
         v-for="item in data.files"
         :key="item.name"
         @click="fileSelected(item.name, item.path)"
         v-bind:class="{selected: (selectedFiles[item.path] !== undefined && selectedFiles[item.path] !== null)}">
        <v-list-tile-action>
          <v-icon class="whiteAfterSelected">insert_drive_file</v-icon>
        </v-list-tile-action>
        <v-list-tile-content>
          <v-list-tile-title class="whiteAfterSelected">{{ item.name }}</v-list-tile-title>
          <v-list-tile-sub-title class="whiteAfterSelected">{{ item.path }}</v-list-tile-sub-title>
        </v-list-tile-content>
      </v-list-tile>
    </v-list>
    <v-divider></v-divider>
    <div v-if="Object.keys(selectedFiles).length > 0">
      <v-card-title>Selected files:</v-card-title>
      <v-card-text
        class="my-0 py-0"
        v-for="item in selectedFiles"
        :key="item.name">
        {{item.path}}
      </v-card-text>
      <v-btn class="selectBtn d-block mx-auto" color="primary" @click="upload">Select</v-btn>
    </div>
  </fc-tile>
</template>

<script>
  import tile from '@/components/tile/fc_tile'
  export default {
    name: "serverBrowser",
    components: {
      'fc-tile': tile
    },
    mounted: function () {
      this.loadDirectory('/')
    },
    methods: {
      loadDirectory: function (path) {
        this.data = null
        this.axios.get(this.$serverAddr + '/directory', {
          params: {
            path: path
          }
        })
        .then((response) => {
          this.data = response.data
        })
      },
      fileSelected: function (name, path) {
        if (this.selectedFiles[path] === undefined || this.selectedFiles[path] === null) {
          this.$set(this.selectedFiles, path, {
            name: name,
            path: path
          })
        } else {
          this.$delete(this.selectedFiles,path)
        }
      },
      upload: function () {
        this.adding = true
        var files = Object.keys(this.selectedFiles).map(function(key) {
          return {
            "name": this.selectedFiles[key]["name"],
            "path": this.selectedFiles[key]["path"]
          };
        }.bind(this));
        this.axios.post(this.$serverAddr + '/dictionary/fromFile', {
          "files": files
        }).then((response) => {
          this.adding = false
          this.$emit('filesuploaded', true)
        }).catch((error) => {
          this.adding = false
          this.$emit('filesuploaded', false)
        })
      }
    },
    data: function () {
      return {
        data: null,
        selectedFiles:{},
        adding: false
      }
    }
  }
</script>

<style scoped>
  .selected {
    background: rgba(37, 157, 173, 0.85) !important;
  }

  .selected .whiteAfterSelected {
    color: white !important;
  }

  .dictContentContainer {
    max-width: 500px;
  }


  .filesCont {
    max-height: 500px;
    overflow: auto;
  }
</style>
