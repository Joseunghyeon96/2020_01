using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;
using System;

public class GameManager : MonoBehaviour
{
    public GameObject[] block = new GameObject[3];

    public int[,] arr = new int[3, 3];

    public int score;
    public Text countText;

    private float time = 20.0f;
    public Text timeText;
    // Start is called before the first frame update

    public GameObject particle_success;
    void Start()
    {
        score = 0;

        arr[0, 0] = 0;
        arr[0, 1] = 0;
        arr[0, 2] = 0;

        arr[1, 0] = 1;
        arr[1, 1] = 1;
        arr[1, 2] = 1;

        arr[2, 0] = 2;
        arr[2, 1] = 2;
        arr[2, 2] = 2;

        for (int x = 0; x < 3; x++)
        {
            for (int y = 0; y < 3; y++)
            {
                Debug.Log("[x,y]" + arr[x, y]);

                float xPos = x * 1.0f - 1.5f;
                float yPos = y * 1.0f - 1.5f;

                if (arr[x, y] == 0)
                {
                    Instantiate(block[0], new Vector3(xPos, yPos, 0.0f), Quaternion.identity);
                }
                else if (arr[x, y] == 1)
                {
                    Instantiate(block[1], new Vector3(xPos, yPos, 0.0f), Quaternion.identity);
                }
                else if (arr[x, y] == 2)
                {
                    Instantiate(block[2], new Vector3(xPos, yPos, 0.0f), Quaternion.identity);
                }
            }
        }
    }

    // Update is called once per frame
    void Update()
    {
        Collider2D clickCol = null;
        Vector3 worldPos;
        Vector2 clickPos;

        if (time >= 1)
        {
            time -= Time.deltaTime;
            int temp = (int)time;
            timeText.text = "Time : " + temp.ToString();
        }
        else
        {
            SceneManager.LoadScene("ending");
        }
        if (Input.GetMouseButtonDown(0))
        {
            worldPos = Camera.main.ScreenToWorldPoint(Input.mousePosition);
            clickPos = new Vector2(worldPos.x, worldPos.y);
            clickCol = Physics2D.OverlapPoint(clickPos);


            RaycastHit2D hit = Physics2D.Raycast(worldPos, transform.forward, 100);
            Debug.DrawRay(worldPos, transform.forward * 10, Color.red, 0.3f);

            if(hit)
            {
                hit.transform.GetComponent<SpriteRenderer>().color = Color.red;
                Instantiate(particle_success, new Vector2(clickPos.x, clickPos.y), Quaternion.identity);
            }
            try
            {
                if (clickCol.tag == "Block1")
                {
                    score += 1;
                    countText.text = "Count: " + score.ToString();
                    Debug.Log("Block1");
                }
                else if (clickCol.tag == "Block2")
                {
                    score += 2;
                    countText.text = "Count: " + score.ToString();
                    Debug.Log("Block2");
                }
                else if (clickCol.tag == "Block3")
                {
                    score += 3;
                    countText.text = "Count: " + score.ToString();
                    Debug.Log("Block3");
                }
                else
                {
                    Debug.Log("No object!");
                }

            }
            catch (NullReferenceException ex)
            {
                Debug.Log(ex);
            }
        }
        if (score >= 20)
        {
            SceneManager.LoadScene("ClearScene");
        }
    }
    
}
