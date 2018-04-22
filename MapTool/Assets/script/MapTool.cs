using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;
using UnityEditor;

[System.Serializable]
public class Data
{
	public string name;
	public Vector3 position;
	public Vector3 rotation;
	public Vector3 scale;
}


public class MapTool : MonoBehaviour {

	public string filename;
	public GameObject[] objects;
	// Use this for initialization
	void Start () {
		FileStream f = new FileStream (Application.dataPath+ "/" + filename, FileMode.Create);
		StreamWriter writer = new StreamWriter (f, System.Text.Encoding.Unicode);
		Data mapData = new Data();

		foreach (GameObject data in objects) {
			mapData.name = data.tag;
			mapData.position = data.transform.position;
			mapData.rotation = new Vector3(data.transform.rotation.x,data.transform.rotation.y, data.transform.rotation.z);
			mapData.scale = data.transform.localScale;
			string json = JsonUtility.ToJson (mapData);
			writer.WriteLine (json);
		}
		writer.Close ();

        EditorUtility.DisplayDialog("Map 파일이 생성되었습니다.", Application.dataPath+"/" + filename, "");
    }
	
	// Update is called once per frame
	void Update () {
		
	}
}
