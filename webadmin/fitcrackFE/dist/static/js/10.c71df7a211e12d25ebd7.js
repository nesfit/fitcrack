webpackJsonp([10],{485:function(e,t,n){"use strict";function a(e){n(867)}Object.defineProperty(t,"__esModule",{value:!0});var i=n(658),r=n(869),o=n(39),s=a,l=o(i.a,r.a,!1,s,"data-v-14ca1364",null);t.default=l.exports},505:function(e,t,n){"use strict";t.a={name:"FcTile",props:{title:String,loading:Boolean,icon:String}}},507:function(e,t,n){"use strict";var a=n(505),i=n(510),r=n(39),o=r(a.a,i.a,!1,null,null,null);t.a=o.exports},508:function(e,t,n){"use strict";t.a=function(e){var t=arguments.length>1&&void 0!==arguments[1]?arguments[1]:{},n=arguments.length>2&&void 0!==arguments[2]?arguments[2]:"en-US";return new Intl.NumberFormat(n,t).format(parseFloat(e))}},509:function(e,t,n){e.exports={default:n(515),__esModule:!0}},510:function(e,t,n){"use strict";var a=function(){var e=this,t=e.$createElement,n=e._self._c||t;return n("v-card",{staticClass:"mx-auto",attrs:{flat:""}},[n("v-card-title",[e.icon?n("v-icon",{attrs:{left:""}},[e._v("\n      "+e._s(e.icon)+"\n    ")]):e._e(),e._v("\n    "+e._s(e.title)+"\n  ")],1),e._v(" "),n("v-card-text",{staticClass:"contentFcTile"},[e.loading?n("v-skeleton-loader",{staticClass:"mx-auto",attrs:{type:"article"}}):e._t("default")],2)],1)},i=[],r={render:a,staticRenderFns:i};t.a=r},511:function(e,t,n){"use strict";function a(e){return e&&e.__esModule?e:{default:e}}t.__esModule=!0;var i=n(512),r=a(i),o=n(509),s=a(o);t.default=function(){function e(e,t){var n=[],a=!0,i=!1,r=void 0;try{for(var o,l=(0,s.default)(e);!(a=(o=l.next()).done)&&(n.push(o.value),!t||n.length!==t);a=!0);}catch(e){i=!0,r=e}finally{try{!a&&l.return&&l.return()}finally{if(i)throw r}}return n}return function(t,n){if(Array.isArray(t))return t;if((0,r.default)(Object(t)))return e(t,n);throw new TypeError("Invalid attempt to destructure non-iterable instance")}}()},512:function(e,t,n){e.exports={default:n(513),__esModule:!0}},513:function(e,t,n){n(199),n(40),e.exports=n(514)},514:function(e,t,n){var a=n(41),i=n(3)("iterator"),r=n(8);e.exports=n(2).isIterable=function(e){var t=Object(e);return void 0!==t[i]||"@@iterator"in t||r.hasOwnProperty(a(t))}},515:function(e,t,n){n(199),n(40),e.exports=n(516)},516:function(e,t,n){var a=n(5),i=n(42);e.exports=n(2).getIterator=function(e){var t=i(e);if("function"!=typeof t)throw TypeError(e+" is not iterable!");return a(t.call(e))}},519:function(e,t,n){"use strict";var a=n(526),i=n.n(a),r=n(509),o=n.n(r),s=n(511),l=n.n(s);t.a={name:"FileUploader",props:{multiple:Boolean,url:{type:String,default:this.$serverAddr},noUpload:Boolean,label:{type:String,default:"Select files"},args:{type:Object,default:function(){}}},data:function(){return{selectedFiles:[],files:null,progress:0,showProgress:!1,fileUploaded:!1}},computed:{overSizeLimit:function(){return this.selectedFiles.some(function(e){return e.size>2e9})}},methods:{uploadChange:function(e){this.progress=Math.round(100*e.loaded/e.total),console.log(this.progress)},fileChange:function(e){this.files=new FormData,this.selectedFiles=[];for(var t=0;t<e.length;t++)this.selectedFiles.push({name:e[t].name,type:e[t].type||"n/a",size:e[t].size,modified:e[t].lastModifiedDate?e[t].lastModifiedDate.toLocaleDateString():"n/a"}),this.files.append("file",e[t],e[t].name);if(this.args){var n=!0,a=!1,r=void 0;try{for(var s,c=o()(i()(this.args));!(n=(s=c.next()).done);n=!0){var d=s.value,u=l()(d,2),f=u[0],p=u[1];this.files.append(f,p)}}catch(e){a=!0,r=e}finally{try{!n&&c.return&&c.return()}finally{if(a)throw r}}}this.fileUploaded=!1,this.progress=0,this.$emit("filesChanged",e)},upload:function(){var e=this;this.showProgress=!0;var t={withCredentials:!0,headers:{"Content-type":"multipart/form-data"},onUploadProgress:this.uploadChange};this.axios.post(this.url,this.files,t).then(function(t){e.fileUploaded=!0,e.$emit("uploadComplete",t.data),e.files=null}).catch(function(e){console.log(e)})}}}},525:function(e,t,n){"use strict";t.a={props:{selectAll:{type:Boolean,default:!1},value:{type:Array,default:function(){return[]}}},data:function(){return{items:[],loading:!1,search:"",selected:[]}},mounted:function(){this.getData(),this.value&&(this.selected=this.value)},methods:{getData:function(){console.error("Selector component missing getData method")},updateSelected:function(){this.$emit("input",this.selected)}},watch:{value:function(e){this.selected=e}}}},526:function(e,t,n){e.exports={default:n(527),__esModule:!0}},527:function(e,t,n){n(528),e.exports=n(2).Object.entries},528:function(e,t,n){var a=n(6),i=n(529)(!0);a(a.S,"Object",{entries:function(e){return i(e)}})},529:function(e,t,n){var a=n(23),i=n(24),r=n(201).f;e.exports=function(e){return function(t){for(var n,o=i(t),s=a(o),l=s.length,c=0,d=[];l>c;)r.call(o,n=s[c++])&&d.push(e?[n,o[n]]:o[n]);return d}}},531:function(e,t,n){"use strict";function a(e){n(532)}var i=n(519),r=n(534),o=n(39),s=a,l=o(i.a,r.a,!1,s,"data-v-1c1e87be",null);t.a=l.exports},532:function(e,t,n){var a=n(533);"string"==typeof a&&(a=[[e.i,a,""]]),a.locals&&(e.exports=a.locals);n(471)("60faad44",a,!0,{})},533:function(e,t,n){t=e.exports=n(470)(!0),t.push([e.i,".uploadInput[data-v-1c1e87be]{max-width:250px}.form[data-v-1c1e87be]{max-width:100%;text-align:center}.uploadButton[data-v-1c1e87be]{margin-top:15px}.spacer[data-v-1c1e87be]{width:10px}","",{version:3,sources:["/home/runner/work/fitcrack/fitcrack/webadmin/fitcrackFE/src/components/fileUploader/fileUploader.vue"],names:[],mappings:"AACA,8BACE,eAAiB,CAClB,AACD,uBACE,eAAgB,AAChB,iBAAmB,CACpB,AACD,+BACE,eAAiB,CAClB,AACD,yBACE,UAAY,CACb",file:"fileUploader.vue",sourcesContent:["\n.uploadInput[data-v-1c1e87be] {\n  max-width: 250px;\n}\n.form[data-v-1c1e87be] {\n  max-width: 100%;\n  text-align: center;\n}\n.uploadButton[data-v-1c1e87be] {\n  margin-top: 15px;\n}\n.spacer[data-v-1c1e87be] {\n  width: 10px;\n}\n"],sourceRoot:""}])},534:function(e,t,n){"use strict";var a=function(){var e=this,t=e.$createElement,n=e._self._c||t;return n("div",{staticClass:"pt-0 mt-0"},[e.overSizeLimit?n("v-alert",{attrs:{type:"error",tile:""}},[e._v("\n    Maximum size of uploaded file is 2 GB.\n  ")]):e._e(),e._v(" "),n("form",{staticClass:"form pa-2",attrs:{enctype:"multipart/form-data"}},[n("v-file-input",{staticClass:"mr-2",attrs:{outlined:"",chips:"","show-size":"",label:e.label,name:"file",multiple:e.multiple},on:{change:e.fileChange}}),e._v(" "),e.noUpload?e._e():n("v-btn",{attrs:{color:"primary",outlined:"",disabled:null===e.files||e.overSizeLimit},on:{click:function(t){return e.upload()}}},[e._v("\n      Upload\n      "),n("v-icon",{attrs:{right:""}},[e._v("\n        mdi-upload\n      ")])],1)],1),e._v(" "),n("v-progress-linear",{attrs:{query:!0,active:e.showProgress,color:"primary"},model:{value:e.progress,callback:function(t){e.progress=t},expression:"progress"}})],1)},i=[],r={render:a,staticRenderFns:i};t.a=r},560:function(e,t,n){"use strict";var a=n(563),i=n(585),r=n(39),o=r(a.a,i.a,!1,null,null,null);t.a=o.exports},563:function(e,t,n){"use strict";var a=n(508),i=n(525);t.a={name:"DictionarySelector",mixins:[i.a],data:function(){return{headers:[{text:"Name",align:"start",value:"name"},{text:"Keyspace",value:"keyspace",align:"end"},{text:"Time",value:"time",align:"end"}]}},methods:{fmt:a.a,getData:function(){var e=this;this.loading=!0,this.axios.get(this.$serverAddr+"/dictionary").then(function(t){e.items=t.data.items,e.loading=!1})}}}},585:function(e,t,n){"use strict";var a=function(){var e=this,t=e.$createElement,n=e._self._c||t;return n("v-data-table",{attrs:{headers:e.headers,items:e.items,search:e.search,"item-key":"id","show-select":"","single-select":!e.selectAll},on:{input:e.updateSelected},scopedSlots:e._u([{key:"item.name",fn:function(t){var a=t.item;return[n("router-link",{attrs:{to:{name:"dictionaryDetail",params:{id:a.id}}}},[e._v("\n      "+e._s(a.name)+"\n      "),n("v-icon",{attrs:{small:"",color:"primary"}},[e._v("\n        mdi-open-in-new\n      ")])],1)]}},{key:"item.keyspace",fn:function(t){var n=t.item;return[e._v("\n    "+e._s(e.fmt(n.keyspace))+"\n  ")]}},{key:"item.time",fn:function(t){var n=t.item;return[e._v("\n    "+e._s(e.$moment(n.time).format("DD.MM.YYYY HH:mm"))+"\n  ")]}}]),model:{value:e.selected,callback:function(t){e.selected=t},expression:"selected"}})},i=[],r={render:a,staticRenderFns:i};t.a=r},586:function(e,t,n){"use strict";var a=n(560),i=n(531);t.a={components:{dictSelector:a.a,fileUploader:i.a},props:{value:Boolean,title:{type:String,default:"Upload or make from dictionary"},uploadUrl:{type:String,default:"#"},working:Boolean},data:function(){return{active:null,newName:"",customName:!1,selectedDict:[]}},computed:{open:{get:function(){return this.value},set:function(e){this.$emit("input",e)}}},watch:{selectedDict:function(e){0==this.newName.length&&(this.customName=!1),e.length>0&&!this.customName&&(this.newName=e[0].name)}},methods:{fileUploaded:function(){this.$emit("fileUploaded"),this.open=!1},dictionarySelected:function(){this.$emit("dictionarySelected",this.selectedDict[0].id,this.newName)}}}},600:function(e,t,n){"use strict";var a=n(586),i=n(601),r=n(39),o=r(a.a,i.a,!1,null,null,null);t.a=o.exports},601:function(e,t,n){"use strict";var a=function(){var e=this,t=e.$createElement,n=e._self._c||t;return n("v-dialog",{attrs:{"max-width":"600"},model:{value:e.open,callback:function(t){e.open=t},expression:"open"}},[n("v-card",[n("v-card-title",{staticClass:"headline"},[e._v("\n      "+e._s(e.title)+"\n    ")]),e._v(" "),n("v-tabs",{model:{value:e.active,callback:function(t){e.active=t},expression:"active"}},[n("v-tab",{attrs:{ripple:""}},[e._v("\n        Upload file\n      ")]),e._v(" "),n("v-tab",{attrs:{ripple:""}},[e._v("\n        Make from dictionary\n      ")]),e._v(" "),n("v-tab-item",[n("v-card",{attrs:{text:""}},[n("file-uploader",{attrs:{url:e.uploadUrl},on:{uploadComplete:e.fileUploaded}})],1)],1),e._v(" "),n("v-tab-item",[e.working?n("div",{staticClass:"text-center"},[n("v-progress-circular",{staticClass:"ma-5",attrs:{size:"50",width:3,indeterminate:"",color:"primary"}})],1):n("v-card",{attrs:{flat:""}},[n("dict-selector",{model:{value:e.selectedDict,callback:function(t){e.selectedDict=t},expression:"selectedDict"}}),e._v(" "),n("v-card-actions",[n("v-text-field",{staticClass:"mr-2",attrs:{outlined:"",dense:"","hide-details":"",label:"Name"},on:{input:function(t){e.customName=!0}},model:{value:e.newName,callback:function(t){e.newName=t},expression:"newName"}}),e._v(" "),n("v-btn",{attrs:{color:"primary",outlined:"",disabled:0==e.selectedDict.length||""===e.newName},on:{click:e.dictionarySelected}},[e._v("\n              Make from dictionary\n            ")])],1)],1)],1)],1)],1)],1)},i=[],r={render:a,staticRenderFns:i};t.a=r},658:function(e,t,n){"use strict";var a=n(508),i=n(507),r=n(600);t.a={name:"PcfgView",components:{fileCreator:r.a,"fc-tile":i.a},data:function(){return{working:!1,dialog:!1,loading:!1,headers:[{text:"Name",align:"left",value:"name"},{text:"Keyspace",value:"keyspace",align:"start"},{text:"Added",value:"time_added",align:"end"},{text:"Actions",value:"actions",align:"end",sortable:!1}],pcfg:[]}},mounted:function(){this.loadPcfg()},methods:{fmt:a.a,loadPcfg:function(){var e=this;this.loading=!0,this.axios.get(this.$serverAddr+"/pcfg",{}).then(function(t){e.pcfg=t.data,e.loading=!1})},makePcfgFromDictionary:function(e,t){var n=this;this.working=!0,this.axios.post(this.$serverAddr+"/pcfg/makeFromDictionary",{dictionary_id:e,name:t}).then(function(e){n.working=!1,n.dialog=!1,n.loadPcfg()}).catch(function(e){n.working=!1})},deletePcfg:function(e){var t=this;this.$root.$confirm("Delete","This will remove "+e.name+" from your PCFGs. Are you sure?").then(function(n){t.loading=!0,t.axios.delete(t.$serverAddr+"/pcfg/"+e.id).then(function(e){t.loadPcfg()})})}}}},867:function(e,t,n){var a=n(868);"string"==typeof a&&(a=[[e.i,a,""]]),a.locals&&(e.exports=a.locals);n(471)("00b52033",a,!0,{})},868:function(e,t,n){t=e.exports=n(470)(!0),t.push([e.i,".noEvent[data-v-14ca1364]{pointer-events:none;display:inline-block}.dz-message[data-v-14ca1364]{cursor:pointer;text-align:center}.max700[data-v-14ca1364]{max-width:700px}","",{version:3,sources:["/home/runner/work/fitcrack/fitcrack/webadmin/fitcrackFE/src/components/pcfg/pcfgView.vue"],names:[],mappings:"AACA,0BACE,oBAAqB,AACrB,oBAAsB,CACvB,AACD,6BACE,eAAgB,AAChB,iBAAmB,CACpB,AACD,yBACE,eAAiB,CAClB",file:"pcfgView.vue",sourcesContent:["\n.noEvent[data-v-14ca1364] {\n  pointer-events: none;\n  display: inline-block;\n}\n.dz-message[data-v-14ca1364] {\n  cursor: pointer;\n  text-align: center;\n}\n.max700[data-v-14ca1364] {\n  max-width: 700px;\n}\n\n"],sourceRoot:""}])},869:function(e,t,n){"use strict";var a=function(){var e=this,t=e.$createElement,n=e._self._c||t;return n("v-container",{staticClass:"max700"},[n("fc-tile",{staticClass:"ma-2",attrs:{title:"PCFGs",icon:e.$route.meta.icon}},[n("v-alert",{staticClass:"mb-0",attrs:{tile:"",text:"",type:"warning"}},[e._v("\n      PCFG files must have a .zip extension.\n    ")]),e._v(" "),n("v-data-table",{attrs:{headers:e.headers,items:e.pcfg.items,loading:e.loading,"footer-props":{itemsPerPageOptions:[10,25,50],itemsPerPageText:"PCFGs per page"}},scopedSlots:e._u([{key:"item.name",fn:function(t){var a=t.item,i=a.id,r=a.name;return[n("router-link",{staticClass:"font-weight-bold",attrs:{to:{name:"pcfgBrowser",params:{id:i}}}},[e._v("\n          "+e._s(r)+"\n        ")])]}},{key:"item.keyspace",fn:function(t){var n=t.item;return[e._v("\n        "+e._s(e.fmt(n.keyspace))+"\n      ")]}},{key:"item.time_added",fn:function(t){var n=t.item;return[e._v("\n        "+e._s(e.$moment(n.time_added).format("DD.MM.YYYY HH:mm"))+"\n      ")]}},{key:"item.actions",fn:function(t){var a=t.item;return[n("v-tooltip",{attrs:{top:""},scopedSlots:e._u([{key:"activator",fn:function(t){var i=t.on;return[n("a",e._g({attrs:{href:e.$serverAddr+"/pcfg/"+a.id,target:"_blank",download:""}},i),[n("v-btn",{attrs:{icon:""}},[n("v-icon",[e._v("mdi-file-download-outline")])],1)],1)]}}],null,!0)},[e._v(" "),n("span",[e._v("Download")])]),e._v(" "),n("v-tooltip",{attrs:{top:""},scopedSlots:e._u([{key:"activator",fn:function(t){var i=t.on;return[n("v-btn",e._g({attrs:{icon:""},on:{click:function(t){return e.deletePcfg(a)}}},i),[n("v-icon",{attrs:{color:"error"}},[e._v("\n                mdi-delete-outline\n              ")])],1)]}}],null,!0)},[e._v(" "),n("span",[e._v("Delete")])])]}}])}),e._v(" "),n("div",{staticClass:"text-right pa-3"},[n("v-btn",{staticClass:"d-inline-block",attrs:{color:"primary",outlined:""},nativeOn:{click:function(t){t.stopPropagation(),e.dialog=!0}}},[e._v("\n        Add new\n      ")])],1)],1),e._v(" "),n("file-creator",{attrs:{title:"Add new PCFG grammar","upload-url":this.$serverAddr+"/pcfg/add",working:e.working},on:{fileUploaded:e.loadPcfg,dictionarySelected:e.makePcfgFromDictionary},model:{value:e.dialog,callback:function(t){e.dialog=t},expression:"dialog"}})],1)},i=[],r={render:a,staticRenderFns:i};t.a=r}});
//# sourceMappingURL=10.c71df7a211e12d25ebd7.js.map