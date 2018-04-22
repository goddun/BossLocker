using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEditor;
using UnityEngine;

public class rawParser : MonoBehaviour {

    void Start()
    {
        Debug.Log("PNG 헤이트맵 생성");

        TerrainData terrainData = null;
        Terrain terrain = null;

        if (Selection.activeGameObject)
            terrain = Selection.activeGameObject.GetComponent<Terrain>();

        if (!terrain)
            terrain = Terrain.activeTerrain;

        if (terrain)
            terrainData = terrain.terrainData;

        if (terrainData == null)
        {
            EditorUtility.DisplayDialog("터레인이 없습니다.", "터레인을 만들어주세요.", "Cancel");
            return;
        }

        int height = terrainData.heightmapHeight;
        int width = terrainData.heightmapWidth;

        Debug.Log("높이: " + height);
        Debug.Log("넓이: " + width);

        byte[] bytes = new byte[height * width];

        float[,] rawHeights = terrainData.GetHeights(0, 0, width, height);

        for (var y = 0; y < height; ++y)
        {
            for (var x = 0; x < width; ++x)
            {
               bytes[y * width + x] = (byte)(rawHeights[y, x] * 256.0f);

                // OpenGL 전용 뒤집기 (z 축이 다름)
                //bytes[y * width + x] = (byte)(rawHeights[(height - y - 1), x] * 256.0f);
            }
        }

        string filename = Application.dataPath + "/" + terrainData.name + "_" + width + "x" + height + ".raw";

        var file = System.IO.File.OpenWrite(filename);
        var writer = new System.IO.BinaryWriter(file);
        writer.Write(bytes);

        writer.Close();

        EditorUtility.DisplayDialog("HeightMap이 생성되었습니다.", "(RAW) in Assets/ as: " + filename, "");
    }


    // Update is called once per frame
    void Update()
    {

    }

}
