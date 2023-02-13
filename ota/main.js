var dropZone = document.getElementById("drop_zone");
var files;

dropZone.addEventListener("dragover", function (evt) {
  this.className = "hover";
  evt.preventDefault();
});

dropZone.addEventListener("dragleave", function (evt) {
  this.className = "";
  evt.preventDefault();
});

dropZone.addEventListener("drop", function (evt) {
  this.className = "";
  handleFileSelect(evt);
});

function handleFileSelect(evt) {
  evt.stopPropagation();
  evt.preventDefault();

  files = evt.dataTransfer.files;
  var file = files[0];
  var file_size_auto;
  if (file.size > 1024 * 1024) {
    file_size_auto = (Math.round((file.size * 100) / (1024 * 1024)) / 100).toString() + "MB";
  } else {
    file_size_auto = (Math.round((file.size * 100) / 1024) / 100).toString() + "KB";
  }
  document.getElementById("fileName").innerHTML = "File name: " + file.name;
  document.getElementById("fileType").innerHTML = "File type: " + file.type;
  document.getElementById("fileSize").innerHTML = "File size: " + file_size_auto;
  document.getElementById("fileLastModified").innerHTML = "Last modified: " + file.lastModifiedDate;
  document.getElementById("fileForm").file = files;
}

function handleDragOver(evt) {
  evt.stopPropagation();
  evt.preventDefault();
  evt.dataTransfer.dropEffect = "copy";
}

function submitForm() {
  var input = document.getElementById("fileForm").elements.files;
  input.files = files;
  document.getElementById("fileForm").submit();
  

  
}
