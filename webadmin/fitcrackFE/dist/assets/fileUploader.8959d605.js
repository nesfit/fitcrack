<<<<<<<< HEAD:webadmin/fitcrackFE/dist/assets/fileUploader.8959d605.js
import{n as o}from"./index.d3f441ac.js";var r=function(){var e=this,t=e.$createElement,s=e._self._c||t;return s("div",{staticClass:"pt-0 mt-0"},[e.overSizeLimit?s("v-alert",{attrs:{type:"error",tile:""}},[e._v(" Maximum size of uploaded file is 2 GB. ")]):e._e(),s("form",{staticClass:"form pa-2",attrs:{enctype:"multipart/form-data"}},[s("v-file-input",{staticClass:"mr-2",attrs:{outlined:"",chips:"","show-size":"",label:e.label,name:"file",multiple:e.multiple},on:{change:e.fileChange}}),e.noUpload?e._e():s("v-btn",{attrs:{color:"primary",outlined:"",disabled:e.files===null||e.overSizeLimit},on:{click:function(l){return e.upload()}}},[e._v(" Upload "),s("v-icon",{attrs:{right:""}},[e._v(" mdi-upload ")])],1)],1),s("v-progress-linear",{attrs:{query:!0,active:e.showProgress,color:"primary"},model:{value:e.progress,callback:function(l){e.progress=l},expression:"progress"}})],1)},i=[];const n={name:"FileUploader",props:{multiple:Boolean,url:{type:String,default:window.$serverAddr},noUpload:Boolean,label:{type:String,default:"Select files"},args:{type:Object,default:()=>{}}},data:function(){return{selectedFiles:[],files:null,progress:0,showProgress:!1,fileUploaded:!1}},computed:{overSizeLimit(){return this.selectedFiles.some(e=>e.size>2e9)}},methods:{uploadChange(e){this.progress=Math.round(e.loaded*100/e.total),console.log(this.progress)},fileChange(e){if(this.files=new FormData,this.selectedFiles=[],this.multiple)for(var t=0;t<e.length;t++)this.selectedFiles.push({name:e[t].name,type:e[t].type||"n/a",size:e[t].size}),this.files.append("file",e[t],e[t].name);else this.selectedFiles.push({name:e.name,type:e.type||"n/a",size:e.size}),this.files.append("file",e,e.name);if(this.args)for(let[s,l]of Object.entries(this.args))this.files.append(s,l);this.fileUploaded=!1,this.progress=0,this.$emit("filesChanged",e)},upload(){this.showProgress=!0;var e={withCredentials:!0,headers:{"Content-type":"multipart/form-data"},onUploadProgress:this.uploadChange};this.axios.post(this.url,this.files,e).then(t=>{this.fileUploaded=!0,this.$emit("uploadComplete",t.data),this.files=null}).catch(t=>{console.log(t)})}}},a={};var p=o(n,r,i,!1,d,"1b82cc23",null,null);function d(e){for(let t in a)this[t]=a[t]}var c=function(){return p.exports}();export{c as F};
========
import{n as o}from"./index.c11e1020.js";var r=function(){var e=this,t=e.$createElement,s=e._self._c||t;return s("div",{staticClass:"pt-0 mt-0"},[e.overSizeLimit?s("v-alert",{attrs:{type:"error",tile:""}},[e._v(" Maximum size of uploaded file is 2 GB. ")]):e._e(),s("form",{staticClass:"form pa-2",attrs:{enctype:"multipart/form-data"}},[s("v-file-input",{staticClass:"mr-2",attrs:{outlined:"",chips:"","show-size":"",label:e.label,name:"file",multiple:e.multiple},on:{change:e.fileChange}}),e.noUpload?e._e():s("v-btn",{attrs:{color:"primary",outlined:"",disabled:e.files===null||e.overSizeLimit},on:{click:function(l){return e.upload()}}},[e._v(" Upload "),s("v-icon",{attrs:{right:""}},[e._v(" mdi-upload ")])],1)],1),s("v-progress-linear",{attrs:{query:!0,active:e.showProgress,color:"primary"},model:{value:e.progress,callback:function(l){e.progress=l},expression:"progress"}})],1)},i=[];const n={name:"FileUploader",props:{multiple:Boolean,url:{type:String,default:window.$serverAddr},noUpload:Boolean,label:{type:String,default:"Select files"},args:{type:Object,default:()=>{}}},data:function(){return{selectedFiles:[],files:null,progress:0,showProgress:!1,fileUploaded:!1}},computed:{overSizeLimit(){return this.selectedFiles.some(e=>e.size>2e9)}},methods:{uploadChange(e){this.progress=Math.round(e.loaded*100/e.total),console.log(this.progress)},fileChange(e){if(this.files=new FormData,this.selectedFiles=[],this.multiple)for(var t=0;t<e.length;t++)this.selectedFiles.push({name:e[t].name,type:e[t].type||"n/a",size:e[t].size}),this.files.append("file",e[t],e[t].name);else this.selectedFiles.push({name:e.name,type:e.type||"n/a",size:e.size}),this.files.append("file",e,e.name);if(this.args)for(let[s,l]of Object.entries(this.args))this.files.append(s,l);this.fileUploaded=!1,this.progress=0,this.$emit("filesChanged",e)},upload(){this.showProgress=!0;var e={withCredentials:!0,headers:{"Content-type":"multipart/form-data"},onUploadProgress:this.uploadChange};this.axios.post(this.url,this.files,e).then(t=>{this.fileUploaded=!0,this.$emit("uploadComplete",t.data),this.files=null}).catch(t=>{console.log(t)})}}},a={};var p=o(n,r,i,!1,d,"1b82cc23",null,null);function d(e){for(let t in a)this[t]=a[t]}var c=function(){return p.exports}();export{c as F};
>>>>>>>> master:webadmin/fitcrackFE/dist/assets/fileUploader.24c45618.js
