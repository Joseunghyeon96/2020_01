using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Particle : MonoBehaviour
{
    // Start is called before the first frame update
    private ParticleSystem ps;
    private float time = 5.0f;

    void Start()
    {
        ps = this.GetComponent<ParticleSystem>();    
    }

    // Update is called once per frame
    void Update()
    {
        time -= Time.deltaTime;

        if (time < 0.0f)
            Destroy(gameObject);
    }
}
