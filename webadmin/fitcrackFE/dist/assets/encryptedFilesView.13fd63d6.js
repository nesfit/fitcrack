<<<<<<<< HEAD:webadmin/fitcrackFE/dist/assets/encryptedFilesView.1cdd4aa6.js
import{t as l}from"./fc_tile.217bb9a5.js";import{n as d}from"./index.d3f441ac.js";import"./vendor.6aa46313.js";var c=function(){var e=this,n=e.$createElement,t=e._self._c||n;return t("v-container",{staticClass:"max1000"},[t("fc-tile",{staticClass:"ma-2",attrs:{title:"Encrypted files",loading:e.encryptedFiles===null}},[e.encryptedFiles!==null?t("v-data-table",{attrs:{headers:e.headers,items:e.encryptedFiles.items,"footer-props":{itemsPerPageOptions:[10,25,50],itemsPerPageText:"Files per page"}},scopedSlots:e._u([{key:"item.name",fn:function(s){var r=s.item;return[e._v(" "+e._s(r.name)+" ")]}},{key:"item.time",fn:function(s){var r=s.item;return[e._v(" "+e._s(e.$moment.utc(r.time).local().format("DD.MM.YYYY HH:mm"))+" ")]}},{key:"item.actions",fn:function(s){var r=s.item;return[t("v-tooltip",{attrs:{top:""},scopedSlots:e._u([{key:"activator",fn:function(a){var o=a.on;return[t("a",e._g({attrs:{href:e.$serverAddr+"/protectedFiles/"+r.id,target:"_blank",download:""}},o),[t("v-btn",{attrs:{icon:""}},[t("v-icon",[e._v("mdi-file-download-outline")])],1)],1)]}}],null,!0)},[t("span",[e._v("Download")])])]}}],null,!1,726433026)}):e._e()],1)],1)},u=[];const p={name:"EncryptedFilesView",components:{"fc-tile":l},data:function(){return{uploadOptions:{url:this.$serverAddr+"/protectedFiles/",paramName:"file"},headers:[{text:"Name",align:"start",value:"name"},{text:"Hash",value:"hash",align:"end"},{text:"Time",value:"time",align:"end"},{text:"Actions",value:"actions",align:"end",sortable:!1}],encryptedFiles:[],uploadProgress:0}},mounted:function(){this.loadFiles()},methods:{loadFiles:function(){this.axios.get(this.$serverAddr+"/protectedFiles/",{}).then(e=>{this.encryptedFiles=e.data})},uploadProgressChanged:function(e,n,t){this.uploadProgress=e},uploadComplete:function(e,n,t){n==="error"?this.axios.interceptors.response.handlers["0"].rejected(JSON.parse(t.response)):this.loadFiles()}}},i={};var m=d(p,c,u,!1,_,"7e310374",null,null);function _(e){for(let n in i)this[n]=i[n]}var y=function(){return m.exports}();export{y as default};
========
import{t as l}from"./fc_tile.83a0e997.js";import{n as d}from"./index.c11e1020.js";import"./vendor.6aa46313.js";var c=function(){var e=this,n=e.$createElement,t=e._self._c||n;return t("v-container",{staticClass:"max1000"},[t("fc-tile",{staticClass:"ma-2",attrs:{title:"Encrypted files",loading:e.encryptedFiles===null}},[e.encryptedFiles!==null?t("v-data-table",{attrs:{headers:e.headers,items:e.encryptedFiles.items,"footer-props":{itemsPerPageOptions:[10,25,50],itemsPerPageText:"Files per page"}},scopedSlots:e._u([{key:"item.name",fn:function(s){var r=s.item;return[e._v(" "+e._s(r.name)+" ")]}},{key:"item.time",fn:function(s){var r=s.item;return[e._v(" "+e._s(e.$moment.utc(r.time).local().format("DD.MM.YYYY HH:mm"))+" ")]}},{key:"item.actions",fn:function(s){var r=s.item;return[t("v-tooltip",{attrs:{top:""},scopedSlots:e._u([{key:"activator",fn:function(a){var o=a.on;return[t("a",e._g({attrs:{href:e.$serverAddr+"/protectedFiles/"+r.id,target:"_blank",download:""}},o),[t("v-btn",{attrs:{icon:""}},[t("v-icon",[e._v("mdi-file-download-outline")])],1)],1)]}}],null,!0)},[t("span",[e._v("Download")])])]}}],null,!1,726433026)}):e._e()],1)],1)},u=[];const p={name:"EncryptedFilesView",components:{"fc-tile":l},data:function(){return{uploadOptions:{url:this.$serverAddr+"/protectedFiles/",paramName:"file"},headers:[{text:"Name",align:"start",value:"name"},{text:"Hash",value:"hash",align:"end"},{text:"Time",value:"time",align:"end"},{text:"Actions",value:"actions",align:"end",sortable:!1}],encryptedFiles:[],uploadProgress:0}},mounted:function(){this.loadFiles()},methods:{loadFiles:function(){this.axios.get(this.$serverAddr+"/protectedFiles/",{}).then(e=>{this.encryptedFiles=e.data})},uploadProgressChanged:function(e,n,t){this.uploadProgress=e},uploadComplete:function(e,n,t){n==="error"?this.axios.interceptors.response.handlers["0"].rejected(JSON.parse(t.response)):this.loadFiles()}}},i={};var m=d(p,c,u,!1,_,"7e310374",null,null);function _(e){for(let n in i)this[n]=i[n]}var y=function(){return m.exports}();export{y as default};
>>>>>>>> master:webadmin/fitcrackFE/dist/assets/encryptedFilesView.13fd63d6.js
