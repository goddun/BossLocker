using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;


public class MapTool : MonoBehaviour {

	public string path = "map.txt";
	public GameObject[] objects;
	// Use this for initialization
	void Start () {
		FileStream f = new FileStream ("map.txt", FileMode.Create);
		StreamWriter writer = new StreamWriter (f, System.Text.Encoding.Unicode);

		foreach (GameObject data in objects) {
			writer.WriteLine (data.transform.position +" " +data.tag);
		}
		writer.Close ();
	}
	
	// Update is called once per frame
	void Update () {
		
	}
}
