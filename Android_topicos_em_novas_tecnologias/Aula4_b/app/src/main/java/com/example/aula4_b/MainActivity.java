package com.example.aula4_b;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.ImageView;
import android.widget.RadioGroup;

public class MainActivity extends AppCompatActivity {

    private RadioGroup goleiro, zagueiro, mc1, mc2, atacante;
    private ImageView imgGol, imgZag, imgMc1, imgMc2, imgAtac;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        goleiro = findViewById(R.id.rb_group_gol);
        zagueiro = findViewById(R.id.rb_group_zag);
        mc1 = findViewById(R.id.rb_group_mc_1);
        mc2 = findViewById(R.id.rb_group_mc_2);
        atacante = findViewById(R.id.rb_group_atacante);
        imgGol = findViewById(R.id.img_gol);
        imgZag= findViewById(R.id.img_zag);
        imgMc1 = findViewById(R.id.img_mc_1);
        imgMc2 = findViewById(R.id.img_mc_2);
        imgAtac = findViewById(R.id.img_atacante);

        goleiro.setOnCheckedChangeListener(new RadioGroup.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(RadioGroup group, int checkedId) {
                if(checkedId == R.id.rb_goleiro1){
                    imgGol.setImageResource(R.drawable.buffon);
                } else if(checkedId == R.id.rb_goleiro2){
                    imgGol.setImageResource(R.drawable.degea);
                } else {
                    imgGol.setImageResource(R.drawable.padrao);
                }
            }
        });

        zagueiro.setOnCheckedChangeListener(new RadioGroup.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(RadioGroup group, int checkedId) {
                if(checkedId == R.id.rb_zagueiro_1){
                    imgZag.setImageResource(R.drawable.tsilva);
                } else if(checkedId == R.id.rb_zagueiro_2){
                    imgZag.setImageResource(R.drawable.sergioramos);
                } else{
                    imgZag.setImageResource(R.drawable.padrao);
                }
            }
        });

        mc1.setOnCheckedChangeListener(new RadioGroup.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(RadioGroup group, int checkedId) {
                if(checkedId == R.id.rb_mc_1){
                    imgMc1.setImageResource(R.drawable.kroos);
                } else if(checkedId == R.id.rb_mc_2){
                    imgMc1.setImageResource(R.drawable.modric);
                } else{
                    imgMc1.setImageResource(R.drawable.padrao);
                }
            }
        });

        mc2.setOnCheckedChangeListener(new RadioGroup.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(RadioGroup group, int checkedId) {
                if(checkedId == R.id.rb_mc_3){
                    imgMc2.setImageResource(R.drawable.debruyne);
                } else if(checkedId == R.id.rb_mc_4){
                    imgMc2.setImageResource(R.drawable.coutinho);
                } else{
                    imgMc2.setImageResource(R.drawable.padrao);
                }
            }
        });

        atacante.setOnCheckedChangeListener(new RadioGroup.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(RadioGroup group, int checkedId) {
                if(checkedId == R.id.rb_atacante_1){
                    imgAtac.setImageResource(R.drawable.pique);
                } else if(checkedId == R.id.rb_atacante2){
                    imgAtac.setImageResource(R.drawable.messi);
                } else{
                    imgAtac.setImageResource(R.drawable.padrao);
                }
            }
        });

    }
}
