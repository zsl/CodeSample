package main

import (
	"os"
	"fmt"
	"path/filepath"
	"io/ioutil"
	"net/http"
	"time"
	"strings"
	"archive/zip"
)

const (
	usage = `usage: upobj objpath`
)

const (
	UPLOAD_SUCC = iota
	UPLOAD_ERR_COMM
	UPLOAD_ERR_SERVER
	UPLOAD_ERR_OPENFILE
)

var (
	curtime = time.Now().Format("20060102150405")
	url = "http://60.28.201.13:808/jniobj.lct?key=" + curtime
)

func main(){
	if len(os.Args) < 2 {
		fmt.Printf("%s\n", usage)
		os.Exit(1)
	}

	path := os.Args[1]
	info, err := os.Stat(path)
	if err != nil {
		fmt.Printf("%s", err)
		os.Exit(1)
	}

	var zippath string

	if info.IsDir() {
		tempf, err := ioutil.TempFile("", filepath.Base(path))
		if (err != nil) {
			fmt.Errorf("failed to create temp file")
			os.Exit(1)
		}

		defer os.Remove(tempf.Name())

		succ := mkzipFile(path, tempf)
		tempf.Close()

		if !succ {
			fmt.Errorf("failed to mkzip")
			os.Exit(1)
		}

		zippath = tempf.Name()
	} else if filepath.Ext(path) == ".zip" {
		zippath = path
	} else {
		fmt.Printf("path should be dir or zip file")
		os.Exit(1)
	}

	fmt.Printf("begin upload %s ...\n", zippath)
	ret := upload(zippath)

	if UPLOAD_SUCC == ret {
		fmt.Printf("Success :)")
	} else {
		fmt.Printf("Failed -_-")

		os.Exit(1)
	}

}

func upload(zippath string) int{
	f, err := os.OpenFile(zippath, os.O_RDONLY, 0)
	if err != nil {
		fmt.Errorf("err when upload, %s\n", err)
		return UPLOAD_ERR_OPENFILE
	}
	defer f.Close()

	resp, err := http.Post(url, "application/octet-stream", f)
	if err != nil {
		fmt.Errorf("err when post, %s\n", err)
		return UPLOAD_ERR_COMM
	}

	defer resp.Body.Close()

	if resp.StatusCode != 200 {
		fmt.Errorf("err status code: %d\n", resp.StatusCode)
		return UPLOAD_ERR_COMM
	}

	body, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		fmt.Errorf("err when read body, %s\n", err)
		return UPLOAD_ERR_COMM
	}

	strBody := string(body)

	fmt.Printf("%s\n", strBody)

	if strings.Contains(strBody, "err,") {
		return UPLOAD_ERR_SERVER
	}

	return UPLOAD_SUCC
}

func mkzip(dirPath, outPath string) bool {
	dstFile, err := os.OpenFile(outPath, os.O_WRONLY|os.O_CREATE, 0664)
	if err != nil {
		fmt.Printf("failed when mkzip %s: %s", outPath, err)
		return false
	}

	fmt.Printf("mkzip %s\n", outPath)

	defer dstFile.Close()

	return mkzipFile(dirPath, dstFile)
}

func mkzipFile(dirPath string, dstFile *os.File) bool {
	zf := zip.NewWriter(dstFile)
	defer zf.Close()

	filepath.Walk(dirPath, func(fullpath string, info os.FileInfo, err error) error {
		if err != nil {
			return filepath.SkipDir
		}

		if info.IsDir() {
			return nil
		}

		name, _ := filepath.Rel(dirPath, fullpath)
		name = filepath.ToSlash(name)
		zfWriter, err := zf.Create(name)
		if err != nil {
			fmt.Errorf("err when CreateHeader %s:%s\n", fullpath, err)
			return filepath.SkipDir
		}

		fmt.Printf("write %s\n", name)

		data, err := ioutil.ReadFile(fullpath)
		if err != nil {
			fmt.Errorf("err when ReadFile %s:%s\n", fullpath, err)
			return filepath.SkipDir
		}

		zfWriter.Write(data)

		return nil
	})

	return true
}

