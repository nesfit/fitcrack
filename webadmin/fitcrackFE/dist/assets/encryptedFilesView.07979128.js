import{t as l}from"./fc_tile.bc27a6a7.js";import{n as d}from"./index.0b60da2f.js";import"./vendor.6aa46313.js";var c=function(){var e=this,n=e.$createElement,t=e._self._c||n;return t("v-container",{staticClass:"max1000"},[t("fc-tile",{staticClass:"ma-2",attrs:{title:"Encrypted files",loading:e.encryptedFiles===null}},[e.encryptedFiles!==null?t("v-data-table",{attrs:{headers:e.headers,items:e.encryptedFiles.items,"footer-props":{itemsPerPageOptions:[10,25,50],itemsPerPageText:"Files per page"}},scopedSlots:e._u([{key:"item.name",fn:function(s){var r=s.item;return[e._v(" "+e._s(r.name)+" ")]}},{key:"item.time",fn:function(s){var r=s.item;return[e._v(" "+e._s(e.$moment(r.time).format("DD.MM.YYYY HH:mm"))+" ")]}},{key:"item.actions",fn:function(s){var r=s.item;return[t("v-tooltip",{attrs:{top:""},scopedSlots:e._u([{key:"activator",fn:function(i){var o=i.on;return[t("a",e._g({attrs:{href:e.$serverAddr+"/protectedFiles/"+r.id,target:"_blank",download:""}},o),[t("v-btn",{attrs:{icon:""}},[t("v-icon",[e._v("mdi-file-download-outline")])],1)],1)]}}],null,!0)},[t("span",[e._v("Download")])])]}}],null,!1,3250927404)}):e._e()],1)],1)},u=[];const p={name:"EncryptedFilesView",components:{"fc-tile":l},data:function(){return{uploadOptions:{url:this.$serverAddr+"/protectedFiles/",paramName:"file"},headers:[{text:"Name",align:"start",value:"name"},{text:"Hash",value:"hash",align:"end"},{text:"Time",value:"time",align:"end"},{text:"Actions",value:"actions",align:"end",sortable:!1}],encryptedFiles:[],uploadProgress:0}},mounted:function(){this.loadFiles()},methods:{loadFiles:function(){this.axios.get(this.$serverAddr+"/protectedFiles/",{}).then(e=>{this.encryptedFiles=e.data})},uploadProgressChanged:function(e,n,t){this.uploadProgress=e},uploadComplete:function(e,n,t){n==="error"?this.axios.interceptors.response.handlers["0"].rejected(JSON.parse(t.response)):this.loadFiles()}}},a={};var m=d(p,c,u,!1,_,"14cb3fae",null,null);function _(e){for(let n in a)this[n]=a[n]}var y=function(){return m.exports}();export{y as default};
