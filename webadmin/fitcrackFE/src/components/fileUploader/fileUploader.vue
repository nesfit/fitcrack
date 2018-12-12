<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
    <div class="pt-0 mt-0">
      <v-alert :value="true" type="warning" class="mt-0 mb-0" >
        Maximum size of uploaded file is 2 GB.
      </v-alert>
      <form enctype="multipart/form-data" class="form px-2">
        <input type="file" class="uploadInput pt-2" name="file" :multiple="multiple" v-on:change="fileChange($event.target.files)"/>
        <v-btn v-if="!noUpload" type="button" color="primary" class="uploadButton" outline @click="upload()" :disabled="files === null">
          Upload
          <span class="spacer"></span><v-icon>cloud_upload</v-icon>
        </v-btn>
      </form>
      <ul v-if="multiple">
        <li v-for="file in selectedFiles">
          <strong>{{file.name}}</strong> - {{file.size}} bytes, last modified: {{file.modified}}
        </li>
      </ul>
      <v-progress-linear
        :query="true"
        v-model="progress"
        :active="showProgress"
        color="primary"
      ></v-progress-linear>
    </div>
</template>

<script>
  export default {
    name: "fileUploader",
    props: ['multiple', 'url', 'noUpload'],
    data: function () {
      return {
        selectedFiles: [],
        files: null,
        progress: 0,
        showProgress: false,
        fileUploaded: false
      }
    },
    methods:{
      uploadChange(progressEvent) {
        this.progress = Math.round((progressEvent.loaded * 100) / progressEvent.total)
        console.log(this.progress)
      },
      fileChange(fileList) {
        this.files = new FormData()
        this.selectedFiles = []
        for (var i = 0; i < fileList.length; i++) {
          this.selectedFiles.push({
              'name': fileList[i].name,
              'type': fileList[i].type || 'n/a',
              'size': fileList[i].size,
              'modified': fileList[i].lastModifiedDate ? fileList[i].lastModifiedDate.toLocaleDateString() : 'n/a'
            })
          this.files.append("file", fileList[i], fileList[i].name);
        }
        this.fileUploaded = false
        this.progress = 0
        this.$emit('filesChanged', fileList)
      },
      upload() {
        this.showProgress = true
        var config = {
          withCredentials: true,
          headers: {
            'Content-type': 'multipart/form-data'
          },
          onUploadProgress: this.uploadChange
        }

        this.axios.post(this.url, this.files, config
        ).then(response => {
          this.fileUploaded = true
          this.$emit('uploadComplete', response.data)
          this.files = null
        }).catch(error => {
          console.log(error)
        })
      },
    }
  }
</script>

<style scoped>
.uploadInput {
  max-width: 250px;
}

.form {
  max-width: 100%;
  text-align: center;
}

.uploadButton {
  margin-top: 15px;
}

.spacer {
  width: 10px;
}
</style>
