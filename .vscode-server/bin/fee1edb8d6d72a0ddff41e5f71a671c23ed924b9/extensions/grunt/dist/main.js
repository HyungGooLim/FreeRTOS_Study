(()=>{"use strict";var e={398:e=>{e.exports=require("vscode")},317:e=>{e.exports=require("child_process")},896:e=>{e.exports=require("fs")},928:e=>{e.exports=require("path")}},t={};function s(r){var o=t[r];if(void 0!==o)return o.exports;var i=t[r]={exports:{}};return e[r](i,i.exports,s),i.exports}var r={};(()=>{var e=r;Object.defineProperty(e,"__esModule",{value:!0}),e.activate=function(e){l=new v,l.start()},e.deactivate=function(){l.dispose()};const t=s(928),o=s(896),i=s(317),n=s(398);function a(e){return new Promise(((t,s)=>{o.exists(e,(e=>{t(e)}))}))}const d=["build","compile","watch"];function c(e){for(const t of d)if(-1!==e.indexOf(t))return!0;return!1}const u=["test"];function h(e){for(const t of u)if(-1!==e.indexOf(t))return!0;return!1}let p,l;function f(){return p||(p=n.window.createOutputChannel("Grunt Auto Detection")),p}function k(){n.window.showWarningMessage(n.l10n.t("Problem finding grunt tasks. See the output for more information."),n.l10n.t("Go to output")).then((()=>{f().show(!0)}))}async function g(e){let s;const r=process.platform;return s="win32"===r&&await a(t.join(e,"node_modules",".bin","grunt.cmd"))?t.join(".","node_modules",".bin","grunt.cmd"):"linux"!==r&&"darwin"!==r||!await a(t.join(e,"node_modules",".bin","grunt"))?"grunt":t.join(".","node_modules",".bin","grunt"),s}class w{constructor(e,t){this._workspaceFolder=e,this._gruntCommand=t}get workspaceFolder(){return this._workspaceFolder}isEnabled(){return"on"===n.workspace.getConfiguration("grunt",this._workspaceFolder.uri).get("autoDetect")}start(){const e=t.join(this._workspaceFolder.uri.fsPath,"{node_modules,[Gg]runtfile.js}");this.fileWatcher=n.workspace.createFileSystemWatcher(e),this.fileWatcher.onDidChange((()=>this.promise=void 0)),this.fileWatcher.onDidCreate((()=>this.promise=void 0)),this.fileWatcher.onDidDelete((()=>this.promise=void 0))}async getTasks(){return this.isEnabled()?(this.promise||(this.promise=this.computeTasks()),this.promise):[]}async getTask(e){const t=e.definition,s=t.task;if(s){const e={cwd:this.workspaceFolder.uri.fsPath},r="grunt";return-1===s.indexOf(" ")?new n.Task(t,this.workspaceFolder,s,r,new n.ShellExecution(`${await this._gruntCommand}`,[s,...t.args],e)):new n.Task(t,this.workspaceFolder,s,r,new n.ShellExecution(`${await this._gruntCommand}`,[`"${s}"`,...t.args],e))}}async computeTasks(){const e="file"===this._workspaceFolder.uri.scheme?this._workspaceFolder.uri.fsPath:void 0,s=[];if(!e)return s;if(!await a(t.join(e,"gruntfile.js"))&&!await a(t.join(e,"Gruntfile.js")))return s;const r=`${await this._gruntCommand} --help --no-color`;try{const{stdout:t,stderr:s}=await(o=r,d={cwd:e},new Promise(((e,t)=>{i.exec(o,d,((s,r,o)=>{s&&t({error:s,stdout:r,stderr:o}),e({stdout:r,stderr:o})}))})));s&&(f().appendLine(s),k());const a=[];if(t){const e=t.split(/\r{0,1}\n/);let s=!1,r=!1;for(const t of e)if(0!==t.length)if(s||r){if(s&&!r)if(0===t.indexOf("Tasks run in the order specified"))r=!0;else{const e=/^\s*(\S.*\S)  \S/g.exec(t);if(e&&2===e.length){const t=e[1],s={type:"grunt",task:t},r="grunt",o={cwd:this.workspaceFolder.uri.fsPath},i=-1===t.indexOf(" ")?new n.Task(s,this.workspaceFolder,t,r,new n.ShellExecution(`${await this._gruntCommand} ${t}`,o)):new n.Task(s,this.workspaceFolder,t,r,new n.ShellExecution(`${await this._gruntCommand} "${t}"`,o));a.push(i);const d=t.toLowerCase();c(d)?i.group=n.TaskGroup.Build:h(d)&&(i.group=n.TaskGroup.Test)}}}else 0===t.indexOf("Available tasks")&&(s=!0)}return a}catch(e){const t=f();return e.stderr&&t.appendLine(e.stderr),e.stdout&&t.appendLine(e.stdout),t.appendLine(n.l10n.t("Auto detecting Grunt for folder {0} failed with error: {1}', this.workspaceFolder.name, err.error ? err.error.toString() : 'unknown")),k(),s}var o,d}dispose(){this.promise=void 0,this.fileWatcher&&this.fileWatcher.dispose()}}class v{constructor(){this.detectors=new Map}start(){const e=n.workspace.workspaceFolders;e&&this.updateWorkspaceFolders(e,[]),n.workspace.onDidChangeWorkspaceFolders((e=>this.updateWorkspaceFolders(e.added,e.removed))),n.workspace.onDidChangeConfiguration(this.updateConfiguration,this)}dispose(){this.taskProvider&&(this.taskProvider.dispose(),this.taskProvider=void 0),this.detectors.clear()}updateWorkspaceFolders(e,t){for(const e of t){const t=this.detectors.get(e.uri.toString());t&&(t.dispose(),this.detectors.delete(e.uri.toString()))}for(const t of e){const e=new w(t,g(t.uri.fsPath));this.detectors.set(t.uri.toString(),e),e.isEnabled()&&e.start()}this.updateProvider()}updateConfiguration(){for(const e of this.detectors.values())e.dispose(),this.detectors.delete(e.workspaceFolder.uri.toString());const e=n.workspace.workspaceFolders;if(e)for(const t of e)if(!this.detectors.has(t.uri.toString())){const e=new w(t,g(t.uri.fsPath));this.detectors.set(t.uri.toString(),e),e.isEnabled()&&e.start()}this.updateProvider()}updateProvider(){if(!this.taskProvider&&this.detectors.size>0){const e=this;this.taskProvider=n.tasks.registerTaskProvider("grunt",{provideTasks:()=>e.getTasks(),resolveTask:t=>e.getTask(t)})}else this.taskProvider&&0===this.detectors.size&&(this.taskProvider.dispose(),this.taskProvider=void 0)}getTasks(){return this.computeTasks()}computeTasks(){if(0===this.detectors.size)return Promise.resolve([]);if(1===this.detectors.size)return this.detectors.values().next().value.getTasks();{const e=[];for(const t of this.detectors.values())e.push(t.getTasks().then((e=>e),(()=>[])));return Promise.all(e).then((e=>{const t=[];for(const s of e)s&&s.length>0&&t.push(...s);return t}))}}async getTask(e){if(0!==this.detectors.size){if(1===this.detectors.size)return this.detectors.values().next().value.getTask(e);if(e.scope!==n.TaskScope.Workspace&&e.scope!==n.TaskScope.Global&&e.scope){const t=this.detectors.get(e.scope.uri.toString());if(t)return t.getTask(e)}}}}})();var o=exports;for(var i in r)o[i]=r[i];r.__esModule&&Object.defineProperty(o,"__esModule",{value:!0})})();
//# sourceMappingURL=https://main.vscode-cdn.net/sourcemaps/fee1edb8d6d72a0ddff41e5f71a671c23ed924b9/extensions/grunt/dist/main.js.map