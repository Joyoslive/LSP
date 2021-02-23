using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Json;
using System.Text;
using UnityEditor;
using UnityEngine;

public class SceneReader : MonoBehaviour
{
    [DataContract]
    private struct SeasonShiftObject
    {
        [DataMember] public string name;
        [DataMember] public Vector3 pos;
        [DataMember] public Vector3 rot;
        [DataMember] public Vector3 scale;
        [DataMember] public string meshFileName;
        [DataMember] public string meshDirectory;
        [DataMember] public Vector3 boxCollider;
    }
    
    // Start is called before the first frame update
    void Start()
    {
        var objects = new List<SeasonShiftObject>();

        foreach (var obj in FindObjectsOfType<GameObject>())
        {
            if (obj.name == "Directional Light" || obj.name == "Main Camera" || obj.name == "Player")
                continue;
            var t = obj.transform;
            if (t.parent != null)
            {
                continue;
            }
            
            string path = null;
            foreach (Transform child in t)
            {
                var m = child.GetComponent<MeshFilter>();
                if (m == null)
                    continue;
                path = AssetDatabase.GetAssetPath(m.sharedMesh).Substring(7);
            }

            var col = t.GetComponent<UnityEngine.BoxCollider>();
            objects.Add(new SeasonShiftObject
            {
                name = obj.name,
                pos = t.position,
                rot = t.localEulerAngles,
                scale = t.localScale,
                meshFileName = path != null ? GetFileName(path) : "",
                meshDirectory = path != null ? GetFileDirectory(path) : "",
                boxCollider = col ? col.size : new Vector3(0, 0, 0)
            });
        }

        var serialized = Serialize(objects);
        serialized = new string(serialized.Where(c => c != '\\').ToArray());
        File.WriteAllText("scene.json", serialized);
    }

    private string Serialize<T>(T obj)
    {
        using var stream = new MemoryStream {Position = 0};
        using var writer = JsonReaderWriterFactory.CreateJsonWriter(stream, Encoding.UTF8, true, true);
        
        var ser = new DataContractJsonSerializer(typeof(T));
        ser.WriteObject(writer, obj);
        writer.Flush();
        
        stream.Position = 0;
        using var outStream = new StreamReader(stream);
        return outStream.ReadToEnd();
    }

    private T Deserialize<T>(string obj)
    {
        using var stream = new MemoryStream{Position = 0};
        using var inStream = new StreamWriter(stream);
        inStream.Write(obj);

        stream.Position = 0;
        var ser = new DataContractJsonSerializer(typeof(T));
        return (T) ser.ReadObject(stream);
    }
    
    private int IndexOfLast(string s, char c)
    {
        var last = 0;
        while (true)
        {
            var tmp = s.IndexOf(c, last+1);
            if (tmp == -1) break;
            last = tmp;
        }

        return last;
    }
    private string GetFileName(string path)
    { 
        return path.Substring(IndexOfLast(path, '/') + 1);
    }

    private string GetFileDirectory(string path)
    {
        return path.Substring(0, IndexOfLast(path, '/'));
    }

    
}
